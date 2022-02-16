
#include "../utils/object.h"
#include "../utils/state.h"

#include <librdkafka/rdkafkacpp.h>

#include "./kafkaSubscriber.h"
#include "./kafkaHelper.h"
#include "./constants.h"
#include "./exceptions/kafkaException.h"
#include "./extensions/kafkaTransportContextExtensions.h"
#include "./extensions/kafkaPackageExtensions.h"
#include "../utils/vectorTools.h"

#include <functional>
#include <algorithm>
#include <unordered_map>
#include <regex>
#include <sstream>
#include <iterator>
#include <locale>
#include <regex>
#include <set>
#include <string>


using namespace std;
using namespace Quix;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaSubscriber::KafkaSubscriber( const SubscriberConfiguration& subscriberConfiguration, const OutputTopicConfiguration& topicConfiguration )
:
topicConfiguration_(topicConfiguration),
consumerGroupSet_(subscriberConfiguration.consumerGroupSet),
checkForKeepAlivePackets_(subscriberConfiguration.checkForKeepAlivePackets),
eventCallback_( this ),
rebalanceCallback_( this )
{
    config_ = subscriberConfiguration.toConsumerConfig();
}


void KafkaSubscriber::commit(
    const std::vector<std::shared_ptr<TransportContext>>& transportContexts
)
{
    this->commitOffsetsInternal( transportContexts );
}

std::vector<std::shared_ptr<TransportContext>> KafkaSubscriber::filterCommittedContexts(
    Quix::Object* state, 
    const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter
)
{
    // Figure out the extent of the commit
    const CommittedOffsets* committedOffsets = dynamic_cast<const CommittedOffsets*>(state);
    if( committedOffsets == nullptr ) { throw ArgumentOutOfRangeException( "State must be of type CommittedOffsets" ); }

    if( committedOffsets->error.errCode != RdKafka::ERR_NO_ERROR ) { return std::vector<std::shared_ptr<TransportContext>>(); }

    std::vector<TopicPartitionOffset> commitOffsets;
    for( auto& el : committedOffsets->offsets )
    {
        if( el.error.errCode == RdKafka::ERR_NO_ERROR )
        {
            commitOffsets.push_back( el.topicPartitionOffset );
        }
    }

    // they all had error ?
    if( commitOffsets.size() <= 0 ) { return std::vector<std::shared_ptr<TransportContext>>(); }
    
    std::map<TopicPartition, Offset> topicPartitionMaxOffset;
    for( auto & el : commitOffsets )
    {
        auto topicPartition = el.topicPartition();
        auto it = topicPartitionMaxOffset.find(topicPartition);
        if( it != topicPartitionMaxOffset.end() )
        {
            if( el.offset() < it->second )
            {
                it->second = el.offset();
            }
        }else{
            // insert without extra copy
            topicPartitionMaxOffset.emplace(topicPartition, el.offset());
        }
    }

    std::vector<std::shared_ptr<TransportContext>> ret;

    auto contextOffsets = getPartitionOffsets(contextsToFilter, true);
    for( size_t index = 0; index < contextOffsets.size(); ++index )
    {
        auto & contextOffset = contextOffsets[index];
        for( auto&lco : topicPartitionMaxOffset )
        {
            if( lco.first == contextOffset.topicPartition() && ! ( contextOffset.offset() < lco.second ) )
            {
                ret.push_back( contextsToFilter[index] );
                break;
            }
        }
    }

    return ret;
}

std::vector<std::shared_ptr<TransportContext>> KafkaSubscriber::filterRevokedContexts(
    const Quix::Object* state, 
    const std::vector<std::shared_ptr<TransportContext>>& contexts
)
{
    if( contexts.size() <= 0 ) { return contexts; }
    if ( state == nullptr ) { return contexts; }

    const IRevocationPublisher::OnRevokedEventArgs* args = dynamic_cast<const IRevocationPublisher::OnRevokedEventArgs*>(state);
    if( args != nullptr ) { state = args->state(); }
    const IRevocationPublisher::OnRevokingEventArgs* args2 = dynamic_cast<const IRevocationPublisher::OnRevokingEventArgs*>(state);
    if( args2 != nullptr ) { state = args2->state(); }


    const State<std::vector<TopicPartitionOffset>>* offsetList = dynamic_cast<const State<std::vector<TopicPartitionOffset>>*>(state);
    if( offsetList != nullptr )
    {
        std::vector<std::shared_ptr<TransportContext>> ret;
        for( auto& el : contexts )
        {
            std::string topic;
            int partition;
            int64_t offset;
            if ( KafkaTransportContextExtensions( el.get() ).tryGetKafkaCommitDetails( topic, partition, offset) ) { continue; }

            if( 
                std::find_if( 
                    offsetList->cbegin(), 
                    offsetList->cend(), 
                    [=](
                        const TopicPartitionOffset& y
                    ){ 
                        return y.topic() == topic && y.partition().id == partition && y.offset() == offset; 
                    } 
                ) != offsetList->cend()
            )
            {
                ret.push_back(el);
            }
        }
        return ret;
    }

    return std::vector<std::shared_ptr<TransportContext>>();
}



void KafkaSubscriber::close( )
{

    if ( this->consumer_ == nullptr ) { return; }

    std::lock_guard<std::mutex> guard( consumerLock_ );

    auto& cons = this->consumer_;
    if ( cons == nullptr ) { return; }
    closing_ = true;
    lastRevokeCompleteAction_();

    this->consumer_ = nullptr;

    cons->close();
    delete cons; // can't close before we're done returning from consumer.consume due to AccessViolationException, so
                    // while it would look like we could close consumer sooner than this, we can't really.
    closing_ = false;
}

void KafkaSubscriber::open()
{
    if ( this->consumer_ != nullptr ) { return; }

    std::lock_guard<std::mutex> guard( consumerLock_ );

    if ( this->consumer_ != nullptr ) { return; }
    closing_ = false;
    lastRevokeCancelAction_();

    std::string errstr;
    if( this->config_->set("event_cb", &eventCallback_, errstr) != RdKafka::Conf::ConfResult::CONF_OK )
    {
        std::cerr << "Failed to set event_cb: " << errstr << std::endl;
    }
    if( this->config_->set("rebalance_cb", &rebalanceCallback_, errstr ) != RdKafka::Conf::ConfResult::CONF_OK )
    {
        std::cerr << "Failed to set rebalance_cb: " << errstr << std::endl;
    }

    this->consumer_ = RdKafka::KafkaConsumer::create( this->config_, errstr );
    if ( this->consumer_ == nullptr ) {
        // TODO: logging
        std::cerr << "Failed to create consumer: " << errstr << std::endl;
        return;
//        exit(1);
    }

    std::vector<TopicPartitionOffset> partitions = this->topicConfiguration_.partitions;
    if(!consumerGroupSet_)
    {
        if( partitions.empty() )
        {
            Offset selectedOffset;

            std::string autoOffsetValue;
            RdKafka::Conf::ConfResult res = this->config_->get("auto.offset.reset", autoOffsetValue);
            if( res != RdKafka::Conf::ConfResult::CONF_OK )
            {
                selectedOffset = Offset::Unset;
            }
            else if( autoOffsetValue == "latest" )
            {
                selectedOffset = Offset::End;
            }
            else if( autoOffsetValue == "earliest" )
            {
                selectedOffset = Offset::Beginning;
            }
            else
            {
                throw ArgumentOutOfRangeException("config auto.offset.reset");
            }
        }

        // convert all "Partition Any" to explicit list of partition because Any doesn't work. // See https://github.com/confluentinc/confluent-kafka-dotnet/issues/917
        if(
            std::find_if( 
                partitions.cbegin(), 
                partitions.cend(), 
                []( const TopicPartitionOffset& x ) { return x.partition() == Partition::Any; }
            ) != partitions.cend()
        )
        {
            std::vector<TopicPartitionOffset> newPartitions;

            std::vector<RdKafka::TopicPartition *> kafkaPartitions;
            // todo: catch 
            this->consumer_->assignment(kafkaPartitions);

            for( auto& partition : partitions )
            {
                if( partition.partition() != Partition::Any )
                {
                    std::string errstr;
                    auto topic = RdKafka::Topic::create( this->consumer_, partition.topic(), this->config_, errstr );

                    if ( topic == nullptr ) {
                        throw InvalidOperationException(std::string("Topic creation error: ") + errstr);
                    }

                    RdKafka::Metadata* metadata;
                    auto errCode = this->consumer_->metadata(false, topic, &metadata, 10*1000);
                    if( errCode != RdKafka::ErrorCode::ERR_NO_ERROR )
                    {
                        // todo: EXCEPTION

                        throw std::exception();
                        // throw InvalidOperationException(std::string("Failed to retrieve metadata for topic '{partition.Topic}' with partition {partition.Partition}. Try again or specify partitions explicitly.") + errstr);
                    }

                    const auto topics = metadata->topics();
                    const auto topicMetadata = std::find_if( 
                        topics->cbegin(),
                        topics->cend(),
                        [=](const RdKafka::TopicMetadata* el )
                        {
                            return el->topic() == partition.topic();
                        }
                    );

                    if( topicMetadata == topics->cbegin() )
                    {
                        throw std::exception();                                                
                    }

                    auto partitions = (*topicMetadata)->partitions();
                    for( auto& x : *partitions )
                    {
                        newPartitions.push_back( TopicPartitionOffset( partition.topic(), x->id(), partition.offset() ) );
                    }
                    continue;
                }

                // this.logger.LogTrace("Topic '{0}' with partition {1} requires metadata retrieval", partition.Topic, partition.Partition);


                // unlike in C# the regular client is having the information about metadata

                partitions = newPartitions;
            }

            // only allow OFFSET_BEGINNING, OFFSET_END or absolute offset, see https://github.com/confluentinc/confluent-kafka-python/issues/250
            for (size_t index = 0; index < partitions.size(); index++)
            {
                auto& partition = partitions[index];
                if (partition.offset() == Offset::Unset)
                {
                    // this.logger.LogDebug("Topic '{0}' with partition {1} defaults to end, because neither offset or group was specified.", partition.Topic, partition.Partition);
                    partitions[index] = TopicPartitionOffset(partition.topic(), partition.partition(), Offset::End);
                    continue;
                }
                if (partition.offset() == Offset::Stored)
                {
                    // this.logger.LogWarning("Topic '{0}' with partition {1} is unable to use last stored offset, because group was not specified. Defaulting to end.", partition.Topic, partition.Partition);
                    partitions[index] = TopicPartitionOffset(partition.topic(), partition.partition(), Offset::End);
                }
            }

        }
    }

    if( !partitions.empty() )
    {
        std::vector<RdKafka::TopicPartition*> kafkaPartitions;
        
        for( auto& partition : partitions )
        {
            kafkaPartitions.push_back( RdKafka::TopicPartition::create( partition.topic(), partition.partition().id, partition.offset().value() ) );
        }

        this->consumer_->assign( kafkaPartitions );
    }

    this->disconnected_ = false;
    this->startWorkerThread();
}


template<typename Clock, typename Duration>
std::ostream &operator<<(std::ostream &stream,
  const std::chrono::time_point<Clock, Duration> &time_point) {
  const time_t time = Clock::to_time_t(time_point);
#if __GNUC__ > 4 || \
    ((__GNUC__ == 4) && __GNUC_MINOR__ > 8 && __GNUC_REVISION__ > 1)
  // Maybe the put_time will be implemented later?
  struct tm tm;
  localtime_r(&time, &tm);
  return stream << std::put_time(&tm, "%c"); // Print standard date&time
#else
  char buffer[26];
  ctime_r(&time, buffer);
  buffer[24] = '\0';  // Removes the newline that is added
  return stream << buffer;
#endif
}

/**
 * @brief format a string timestamp from the current time
 */
static void print_time() {
    auto time_point = std::chrono::system_clock::now(); 
    cout << time_point << flush;
};

KafkaSubscriber::KafkaEventCb::KafkaEventCb( KafkaSubscriber* subscriber )
:
subscriber_(subscriber)
{

}


void KafkaSubscriber::KafkaEventCb::event_cb(RdKafka::Event &event)
{

    switch (event.type()) {
        case RdKafka::Event::EVENT_ERROR:
//         if (event.fatal()) {
//             std::cerr << "FATAL ";
// //            run = 0;
//         }
        // std::cerr << "ERROR (" << RdKafka::err2str(event.err())
        //             << "): " << event.str() << std::endl;
            subscriber_->onErrorOccurred( KafkaException( event.err() ) );
        break;

        // case RdKafka::Event::EVENT_STATS:
        //     std::cerr << "\"STATS\": " << event.str() << std::endl;
        // break;

        // case RdKafka::Event::EVENT_LOG:
        // fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(), event.fac().c_str(),
        //         event.str().c_str());
        // break;

        // case RdKafka::Event::EVENT_THROTTLE:
        // std::cerr << "THROTTLED: " << event.throttle_time() << "ms by "
        //             << event.broker_name() << " id " << (int)event.broker_id()
        //             << std::endl;
        // break;

        default:
        print_time();
        std::cerr << "EVENT " << event.type() << " ("
                    << RdKafka::err2str(event.err()) << "): " << event.str()
                    << std::endl;
        break;
    }
}

void KafkaSubscriber::KafkaOffsetCommitCb::offset_commit_cb(RdKafka::ErrorCode err,
                                std::vector<RdKafka::TopicPartition*>&offsets)
{
    std::cerr << "KafkaSubscriber::KafkaOffsetCommitCb::offset_commit_cb(): " << RdKafka::err2str(err) << "\n";

    this->parent_->automaticOffsetCommitedHandler(offsets, err);
}


void KafkaSubscriber::KafkaRebalanceCb::rebalance_cb(RdKafka::KafkaConsumer *consumer,
                    RdKafka::ErrorCode err,
                    std::vector<RdKafka::TopicPartition *> &partitions)
{
    std::cerr << "RebalanceCb: " << RdKafka::err2str(err) << ": ";

    part_list_print(partitions);

    // RdKafka::Error *error      = NULL;
    RdKafka::ErrorCode ret_err = RdKafka::ERR_NO_ERROR;

    if ( err == RdKafka::ERR__ASSIGN_PARTITIONS )
    {
        // if ( consumer->rebalance_protocol() == "COOPERATIVE" )
        //     consumer->incremental_assign(partitions);
        // else
        //     consumer->assign(partitions);
            
        this->parent_->partitionsAssignedHandler(consumer, partitions);
    }
    else if( err == RdKafka::ERR__REVOKE_PARTITIONS ) 
    {

        //  if (consumer->rebalance_protocol() == "COOPERATIVE")
        //    consumer->incremental_unassign(partitions);
        //  else
        //    consumer->unassign();

        std::vector< RdKafka::TopicPartition * > p = partitions;                 

        // TODO: missing assignment_lost
        // if( RdKafka::KafkaConsumer::assignment_lost() )
        // {
        //     this->parent_->partitionsLostHandler(consumer, partitions);
        // }
        // else 
        // {
            this->parent_->partitionsRevokedHandler(consumer, partitions);
        // }
    }

}


void KafkaSubscriber::onErrorOccurred(const KafkaException& exception)
{
    const auto msg = exception.kafkaMessage();

    std::string msgLowerCase;
    msgLowerCase.resize(msg.size());

    // Allocate the destination space
    std::locale loc;

    string test = "Hello World";
    for(auto& c : test)
    {
        msgLowerCase.push_back(tolower(c));
    }

    if ( msgLowerCase.find("disconnect")  != std::string::npos )
    {
        //TODO: check if there is same error string in the librdkafka

        std::smatch base_match;
        if ( std::regex_match(msgLowerCase, base_match, Constants::exceptionMsRegex) )
        {
            if( base_match.size() >= 2 )
            {
                int ms = atoi(  base_match[1].str().c_str() );
                if (ms > 180000)
                {
                    std::cerr << "% Idle connection reaped. >>" << msgLowerCase << "<<" << std::endl;
                    return;
                }
            }
        }

        // this.logger.LogWarning(exception, "Disconnected from kafka. Ignore unless occurs frequently in short period of time as client automatically reconnects.");
        return;
    }
    
    if ( msg.find("Local: Maximum application poll interval (max.poll.interval.ms) exceeded")  != std::string::npos )
    {
        // this.logger.LogWarning(exception, "Processing of package took longer ({0}ms) than configured max.poll.interval.ms ({1}ms). Application was deemed dead/stuck and the consumer left the group so the assigned partitions could be assigned to a live application instance. Consider setting max.poll.interval.ms to a higher number if this a valid use case.", Math.Round(currentPackageProcessTime.Elapsed.TotalMilliseconds), this.config.MaxPollIntervalMs ?? 300000);
        this->disconnected_ = true;
        return;
    }
    
    if ( msg.find("Broker: Static consumer fenced by other consumer with same group.instance.id")  != std::string::npos )
    {
        // this.logger.LogWarning(exception, "Static consumer fenced by other consumer with same group.instance.id '{0}' in group '{1}'.", this.config.GroupInstanceId, this.config.GroupId);
        this->disconnected_ = true;
        return;
    }
    
    if ( msg.find("Broker: Invalid session timeout")  != std::string::npos )
    {
        // this.logger.LogError(exception, "Broker: Invalid session timeout {0}", this.config.SessionTimeoutMs);
        this->disconnected_ = true;
        this->canReconnect_ = false;
        return;
    }
    
    if ( msg.find("Receive failed: Connection timed out (after ")  != std::string::npos )
    {

        std::smatch base_match;
        if ( std::regex_match(msgLowerCase, base_match, Constants::exceptionMsRegex) )
        {
            if( base_match.size() >= 2 )
            {
                int ms = atoi(  base_match[1].str().c_str() );
                if (ms > 7500000)
                {
                    std::cerr << "% Idle connection timed out, Kafka will reconnect." << std::endl;
                    return;
                }
        //         this.logger.LogWarning(exception, $"Connection timed out (after {ms}ms in state UP). Kafka will reconnect.");
        //          return;
            }
        }        
    }
    
    if ( this->errorOccurred.size( ) <= 0 )
    {
        // this.logger.LogError(exception, "Exception receiving from Kafka");
    }
    else
    {
        this->errorOccurred(exception);
    }


}


void KafkaSubscriber::commitOffsets()
{

    std::lock_guard<std::mutex> guard( consumerLock_ );

    if ( this->consumer_ == nullptr )
    {
        throw InvalidOperationException("Unable to commit offsets when output is not open.");
    }

    try
    {
        State<std::vector<TopicPartitionOffset>> positions;

    //     // TODO

        std::vector< RdKafka::TopicPartition * > partitions;
        RdKafka::ErrorCode resp = this->consumer_->assignment( partitions );
        if ( resp != RdKafka::ERR_NO_ERROR ){
            throw KafkaException(resp);
        }

        this->consumer_->position( partitions );
        for( auto& topicPartition : partitions )
        {
            auto offset = topicPartition->offset();

            positions.push_back( TopicPartitionOffset(topicPartition->topic(), topicPartition->partition(), offset ) );
        }
        this->onCommitting(this, OnCommittingEventArgs(&positions));

        // TODO
        KafkaOffsetCommitCb receiver(this);

        // TODO: treat return code as a exception
        this->consumer_->commitSync(&receiver);

        resp = this->consumer_->committed( partitions, 2000 );

        // todo on committed offsets
//        this->onCommitted(this, )


    //     // var positions = new List<TopicPartitionOffset>();
    //     // foreach (var topicPartition in this.consumer.Assignment)
    //     // {
    //     //     var position = this.consumer.Position(topicPartition);
    //     //     if (position != Offset.Unset)
    //     //     {
    //     //         positions.Add(new TopicPartitionOffset(topicPartition, position));
    //     //     }
    //     // }
    //     // this.OnCommitting?.Invoke(this, new OnCommittingEventArgs(positions));
    //     // var committed = this.consumer.Commit();
    //     // this.OnCommitted?.Invoke(this, new OnCommittedEventArgs(GetCommittedOffsets(committed, null)));
    }
    catch (KafkaException ex)
    {
        /// TODO: make state hold the shared_ptr
        State<CommittedOffsets> st = getCommittedOffsets( std::vector<TopicPartitionOffset>() );
        this->onCommitted( 
            this, 
            OnCommittedEventArgs( &st )
        );
    }


    // TODO
}

std::vector<TopicPartitionOffset> KafkaSubscriber::createTopicPartitionOffsetList(const std::vector< RdKafka::TopicPartition * >& kafkaTopicPartition) const
{
    std::vector<TopicPartitionOffset> tpo;
    for( auto& el : kafkaTopicPartition )
    {
        auto partition = el->partition();
        auto offset = el->offset();
        auto topic = el->topic();
        tpo.push_back(TopicPartitionOffset(topic, partition, offset));
    }
    return tpo;
}


void KafkaSubscriber::partitionsAssignedHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions)
{
    auto topicPartitionOffsets = this->createTopicPartitionOffsetList(partitions);

    auto lrs = this->lastRevokingState;
    this->lastRevokeCancelAction_();

    auto assignedPartitions = topicPartitionOffsets;

    if( !lrs.empty() )
    {
        std::map<TopicPartitionOffset, TopicPartitionOffset> sameTopicPartitionsMapToOffset;
        std::map<TopicPartitionOffset, RdKafka::TopicPartition* > sameTopicPartitionsMapToKafkaPartition;
        std::set<TopicPartitionOffset> sameTopicPartitionsSet;
        auto index = 0;
        for( auto& el : topicPartitionOffsets )
        {
            auto tp = TopicPartitionOffset(el.topic(), el.partition());
            sameTopicPartitionsSet.insert(tp);
            sameTopicPartitionsMapToOffset.emplace(tp, el);
            sameTopicPartitionsMapToKafkaPartition.emplace(tp, partitions[index]);
            index++;
        }

        std::set<TopicPartitionOffset> lrsset;
        for( auto& el : this->lastRevokingState )
        {
            lrsset.insert(TopicPartitionOffset(el.topic(), el.partition()));
        }

        std::vector<TopicPartitionOffset> sameTopicPartitions;
        std::set_intersection( sameTopicPartitionsSet.begin(), sameTopicPartitionsSet.end(), lrsset.begin(), lrsset.end(),  std::back_inserter(sameTopicPartitions) );


        std::vector<TopicPartitionOffset> sameAsPreviously;
        std::vector<ShouldSkipConsumeResult> seekFuncs;

        for ( auto& topicPartitionOffset : sameTopicPartitions)
        {
            Offset currentPosition( sameTopicPartitionsMapToOffset[topicPartitionOffset].offset() );

            const auto kafkapartition = sameTopicPartitionsMapToKafkaPartition[topicPartitionOffset];

            std::vector<RdKafka::TopicPartition*> kafkaarr;
            consumer->position(kafkaarr);

            if ( !(currentPosition.value() == kafkapartition->offset()) )
            {
                if( !currentPosition.isSpecial() )
                {

                    seekFuncs.push_back([=](const ConsumerResult& cr){
                    //     this.logger.LogDebug("Seeking partition: {0}", topicPartitionOffset.ToString());
                        auto partition = RdKafka::TopicPartition::create(topicPartitionOffset.topic(), topicPartitionOffset.partition().id, topicPartitionOffset.offset().value());
                        this->consumer_->seek(*partition, -1);

                        auto is = topicPartitionOffset.topic() == partition->topic() && topicPartitionOffset.partition().id == partition->partition() && topicPartitionOffset.offset().value() <= partition->offset(); 
                        delete partition;
                        return is;
                    });
                    // .Add((cr) =>
                    // {
                    //     this.logger.LogDebug("Seeking partition: {0}", topicPartitionOffset.ToString());
                    //     this.consumer.Seek(topicPartitionOffset);
                    //     this.logger.LogDebug("Seeked partition: {0}", topicPartitionOffset.ToString());
                    //     return cr.TopicPartition == topicPartitionOffset.TopicPartition && cr.Offset <= topicPartitionOffset.Offset;
                    // });

                }
                else
                {
                    continue; // Do not add it to same as previous
                }
            }
            sameAsPreviously.push_back(sameTopicPartitionsMapToOffset[topicPartitionOffset]);

        }


        if( seekFuncs.size() > 0 )
        {
            this->seekFunc = [=](const ConsumerResult& cr)
            {
                bool skip = false;
                for ( auto& func : seekFuncs )
                {
                    skip = func(cr) || skip; // order is important
                }

                this->seekFunc = [=](const ConsumerResult& cr){ return true; };
                return skip;
            }; // reset after seeking
        }

        State<std::vector<TopicPartitionOffset>> revoked;
        std::set_difference( lrs.begin(), lrs.end(), sameAsPreviously.begin(), sameAsPreviously.end(), std::back_inserter(revoked) );

        if( revoked.size() > 0 )
        {
            // if (this.logger.IsEnabled(LogLevel.Debug))
            // {
            //     foreach (var partition in revoked)
            //     {
            //         this.logger.LogDebug("Partition revoked: {0}", partition.ToString());
            //     }
            // }
            this->onRevoked(this, OnRevokedEventArgs(&revoked));
        }

        // if (this.logger.IsEnabled(LogLevel.Debug))
        // {
        //     foreach (var partition in sameAsPreviously)
        //     {
        //         assignedPartitions.Remove(partition.TopicPartition);
        //         this.logger.LogDebug("Partition re-assigned: {0}", partition.ToString());
        //     }
        // }


    }

    // TODO
}

void KafkaSubscriber::partitionsLostHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions)
{
    this->lastRevokeCancelAction_();
    this->lastRevokingState.clear();

    // if (this.logger.IsEnabled(LogLevel.Debug))
    // {
    //     foreach (var partition in topicPartitionOffsets)
    //     {
    //         this.logger.LogInformation("Partition revoked: {0}", partition.ToString());
    //     }
    // }

    State<std::vector<TopicPartitionOffset>> topicPartitionOffsets(this->createTopicPartitionOffsetList(partitions));
    this->onRevoked(this, OnRevokedEventArgs(&topicPartitionOffsets));

}

void KafkaSubscriber::partitionsRevokedHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions)
{
    lastRevokingState.clear();
    // if (this.logger.IsEnabled(LogLevel.Debug))
    // {
    //     foreach (var partition in topicPartitionOffsets)
    //     {
    //         this.logger.LogDebug("Partition revoking: {0}", partition.ToString());
    //     }
    // }

    auto topicPartitionOffsets = this->createTopicPartitionOffsetList(partitions);

    State<std::vector<TopicPartitionOffset>> state(topicPartitionOffsets);
    this->onRevoking( this, IRevocationPublisher::OnRevokingEventArgs(&state) );

    for(auto &it : topicPartitionOffsets){
        this->lastRevokingState.push_back(it);
    }


    // shared_ptr to simulate closure because we do use the lambda functions later
    shared_ptr<bool> shouldInvoke(new bool);

    // TODO: add cancellation token

    lastRevokeCancelAction_ = [=](){
        *shouldInvoke = false;
        this->lastRevokeCancelAction_ = [](){};
        this->lastRevokingState.clear();
        this->lastRevokeCompleteAction_ = [](){};
    };

    lastRevokeCompleteAction_ = [=](){
        if(!*shouldInvoke) { return; }
        this->lastRevokeCancelAction_();    //clean itself up

        // if (this.logger.IsEnabled(LogLevel.Debug))
        // {
        //     foreach (var partition in topicPartitionOffsets)
        //     {
        //         this.logger.LogDebug("Partition revoked: {0}", partition.ToString());
        //     }
        // }

        State<std::vector<TopicPartitionOffset>> state(topicPartitionOffsets);
        this->onRevoked(this, IRevocationPublisher::OnRevokedEventArgs(&state) );
    };

    if( this->closing_ || this->enableReAssignedLogic )
    {
        this->lastRevokeCompleteAction_();
    }
    else
    {
        this->lastRevokeCompleteAction_();
        // todo: 
        // Task.Delay(revokeTimeoutPeriodInMs, cts.Token).ContinueWith(t => lastRevokeCompleteAction(),
        //     TaskContinuationOptions.OnlyOnRanToCompletion);        
    }

}

void KafkaSubscriber::automaticOffsetCommitedHandler(const std::vector< RdKafka::TopicPartition * >& partitions, RdKafka::ErrorCode err)
{    
    // if (this.logger.IsEnabled(LogLevel.Trace))
    // {
    //     foreach (var committedOffset in offsets.Offsets)
    //     {
    //         logger.LogTrace("Auto committed offset {0} for Topic {1}, Partition {2}, with result {3}", committedOffset.Offset, committedOffset.Topic, committedOffset.Partition, committedOffset.Error);   
    //     }
    // }

}


CommittedOffsets KafkaSubscriber::getCommittedOffsets( const std::vector<TopicPartitionOffset>& offsets ) const
{
    std::vector<TopicPartitionOffsetError> tpoe;
    for( auto& offset : offsets )
    {
        tpoe.push_back( TopicPartitionOffsetError( offset, KafkaException( RdKafka::ERR_NO_ERROR ) ) );
    }
    return CommittedOffsets( tpoe, KafkaException( RdKafka::ERR_NO_ERROR ) ) ;
    // return CommittedOffsets(offsets.Select(y => new TopicPartitionOffsetError(y, new Error(ErrorCode.NoError))).ToList(), new Error(ErrorCode.NoError));      
}


void KafkaSubscriber::commitOffsets(const std::vector<TopicPartitionOffset>& offsets)
{

    std::lock_guard<std::mutex> guard( consumerLock_ );

    if ( this->consumer_ == nullptr )
    {
        throw InvalidOperationException("Unable to commit offsets when output is not open.");
    }

    // TODO
}

void KafkaSubscriber::addMessage( RdKafka::Message *message  )
{
    auto package = KafkaHelper::fromResult( message );

    if ( this->checkForKeepAlivePackets_ && KafkaPackageExtensions<ByteArrayPackage>( package.get() ).isKeepAlivePackage() )
    {
        // this.logger.LogTrace("KafkaOutput: keep alive message read, ignoring.");
        return;
    }

    try
    {
        this->onNewPackage(package);
    }
    catch(...)
    {
        // TODO
    }
}

void KafkaSubscriber::kafkaPollingThread( )
{
    while(threadShouldBeRunning_)
    {
        RdKafka::Message *message = this->consumer_->consume(100);


        switch (message->err())
        {
            case RdKafka::ERR__TIMED_OUT:
                break;        

            case RdKafka::ERR_NO_ERROR:
                this->addMessage(message);
                break;

            case RdKafka::ERR__PARTITION_EOF:
                /* Last message */
                // if (exit_eof && ++eof_cnt == partition_cnt) {
                // std::cerr << "%% EOF reached for all " << partition_cnt << " partition(s)"
                //             << std::endl;
                // run = 0;
                // }
                break;

            case RdKafka::ERR__UNKNOWN_TOPIC:
            case RdKafka::ERR__UNKNOWN_PARTITION:
                // std::cerr << "Consume failed: " << message->errstr() << std::endl;
                // run = 0;
                break;

            default:
                break;
                /* Errors */
                // std::cerr << "Consume failed: " << message->errstr() << std::endl;
                // run = 0;
        }
        delete message;
    }
}

void KafkaSubscriber::startWorkerThread( )
{
    threadShouldBeRunning_ = true;
    readThread_ = std::thread(&KafkaSubscriber::kafkaPollingThread, this);
}


KafkaSubscriber::~KafkaSubscriber()
{
    threadShouldBeRunning_ = false;
    // this->consumer_->stop();
    readThread_.join();
}
