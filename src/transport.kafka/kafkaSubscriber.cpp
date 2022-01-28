
#include "../utils/object.h"
#include "../utils/state.h"

#include "./kafkaSubscriber.h"
#include "./exceptions/kafkaException.h"
#include "./extensions/kafkaTransportContextExtensions.h"

#include <functional>
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <regex>
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


    // TODO
    return contextsToFilter;
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
                        return y.topic == topic && y.partition.id == partition && y.offset == offset; 
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

    // TODO
    // this.workerTaskCts?.Cancel()

    this->consumer_ = nullptr;


    try
    {
        // TODO
        // this.workerTaskPollFinished.Task?.Wait(-1); // length of the wait depends on how long each message takes to get processed
        // this.workerTask = null;
        // this.workerTaskPollFinished = null;
    }
    catch (std::exception ex)
    {
        // Any exception which happens here is related to worker task itself, not the processing of the msges
        // this.logger.LogDebug(ex, "WorkerTask failed when closing");
    }
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
//        exit(1);
    }

    // TODO: 
    // consumerBuilder.SetErrorHandler(this.ConsumerErrorHandler);
    // consumerBuilder.SetStatisticsHandler(this.ConsumerStatisticsHandler);  // empty
                // consumerBuilder.SetPartitionsAssignedHandler(this.PartitionsAssignedHandler);
                // consumerBuilder.SetPartitionsRevokedHandler(this.PartitionsRevokedHandler);
                // consumerBuilder.SetPartitionsLostHandler(this.PartitionsLostHandler);


    //TODO: 
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
        this->consumer_->commitSync(&receiver);

        resp = this->consumer_->committed( partitions, 2000 );


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
        auto sameTopicPartitions = 
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
        // todo: 
        // Task.Delay(revokeTimeoutPeriodInMs, cts.Token).ContinueWith(t => lastRevokeCompleteAction(),
        //     TaskContinuationOptions.OnlyOnRanToCompletion);        
    }

}

void KafkaSubscriber::automaticOffsetCommitedHandler(const std::vector< RdKafka::TopicPartition * >& partitions, RdKafka::ErrorCode err)
{    
    //TODO    
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

