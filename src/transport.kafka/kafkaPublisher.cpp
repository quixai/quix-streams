
#include "../exceptions/invalidOperationException.h"
#include "./kafkaPublisher.h"
#include "./constants.h"
#include "./kafkaPackageExtensions.h"

#include <functional>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaPublisher::KafkaPublisher(const PublisherConfiguration& publisherConfiguration, InputTopicConfiguration topicConfiguration)
:
setupKeepAlive_([](){}),
timer_([](){}, Timer::INFINITE, Timer::INFINITE, false)
{
    conf_ = publisherConfiguration.toProducerConfig();

    std::function<void()> hbAction(nullptr);
    auto partition = topicConfiguration.partition();
    auto topic = topicConfiguration.topic();
    if ( partition == Partition::Any )
    {
        hbAction_ = [=](){
            // this.logger.LogTrace($"Creating admin client to retrieve metadata for keep alive details");

            std::string errstr;
            auto adminClient = RdKafka::Producer::create(conf_, errstr);
            if ( !producer_ ) {
                std::stringstream ss;
                ss << "Failed to create kafka producer (" << errstr << ")";
                /// TODO: logger
                return;
                // throw InvalidOperationException(ss.str());
            }

            RdKafka::Metadata* metadata;
            RdKafka::Topic *kafkaTopic = RdKafka::Topic::create(adminClient, topic, conf_, errstr);
            if ( !kafkaTopic ) {
                std::stringstream ss;
                ss << "Failed to create kafka topic (" << errstr << ")";
                /// TODO: logger
                return;
                // throw InvalidOperationException(ss.str());
            }

            adminClient->metadata(false, kafkaTopic, &metadata, 10*1000);

            bool found = false;
            for( auto it = metadata->topics()->begin(); it != metadata->topics()->end(); ++it)
            {
                if( (*it)->topic() != topic ) { continue; }


                this->partitionsToKeepAliveWith_.clear();

                /// Group by leader and select first leader
                std::unordered_map<int32_t, const RdKafka::PartitionMetadata *> partitionLeaders;

                for(auto partIt = (*it)->partitions()->begin(); partIt != (*it)->partitions()->end(); ++partIt)
                {
                    auto el = (*partIt);
                    auto leader = el->leader();
                    if( partitionLeaders.find(leader) == partitionLeaders.end() )
                    {
                        partitionLeaders[leader] = el;
                        this->partitionsToKeepAliveWith_.push_back(Partition(el->id()));
                        std::cout<< "Automatic keep alive enabled for '" << el->id() << "'" << std::endl;
                        // logger.LogDebug("Automatic keep alive enabled for '{0}'.", topicPartition);
                    }
                }
                found = true;
                break;
            }

            if( !found )
            {                
                // TODO: logger
                std::cout<< "Keep alive could not be set up for topic " << topic << " due to missing topic metadata." << std::endl;
            }

            delete metadata;

            if( found )
            {
                // this.logger.LogTrace($"Finished retrieving metadata for keep alive details");
                std::cout << "Finished retrieving metadata for keep alive details" << std::endl;
            }
            
            hbAction_ = [](){ }; // because no need to do ever again, really            
        };
    }
    else
    {
        hbAction_ = [this,partition](){
            partitionsToKeepAliveWith_.push_back(partition);
            hbAction_ = [](){};
        };
    }


    // unlike in C# the sending with and without partition can be done via the same code
    produce_ = [this, partition](const std::string& topicName, const Quix::Transport::ByteArray& data){ 
            //TODO
        RdKafka::ErrorCode resp = 
            this->producer_->produce(topicName, partition.id,
                        RdKafka::Producer::RK_MSG_COPY /* Copy payload */,
                        /* Value */
                        reinterpret_cast<char *>(data.data()), data.len(),
                        /* Key */
                        NULL, 0,
                        /* Timestamp (defaults to now) */
                        0,
                        /* Message headers, if any */
                        NULL,
                        /* Per-message opaque value passed to
                            * delivery report */
                        NULL);
        if (resp != RdKafka::ERR_NO_ERROR)
        {
            this->errorHandler(resp);
            // TODO: delete headers
        }
    };

    if (publisherConfiguration.keepConnectionAlive && publisherConfiguration.keepConnectionAliveInterval > 0)
    {
        setupKeepAlive_ = [this](){ this->hbAction_(); };

        /// TODO: this uses Threading interval so once we rewrite Timer, then this needs to stay in separate thread
        timer_.setAction(std::bind(&KafkaPublisher::sendKeepAlive, this));
        timer_.change(Timer::INFINITE, publisherConfiguration.keepConnectionAliveInterval);
    }

};

void KafkaPublisher::errorHandler(RdKafka::ErrorCode errorCode) const
{
    auto errStr = RdKafka::err2str(errorCode);

    if (errStr.find("disconnect") != string::npos) {
        // errStr contains disconnect
        std::smatch base_match;
        if ( std::regex_match(errStr, base_match, Constants::exceptionMsRegex) )
        {
            if( base_match.size() >= 2 )
            {
                int ms = atoi(  base_match[1].str().c_str() );
                if (ms > 180000)
                {
                    std::cerr << "% Idle connection reaped. >>" << errStr << "<<" << std::endl;
                    // this.logger.LogDebug(ex, "Idle connection reaped.");
                    return;
                }
            }

        }

    }

    // this.logger.LogError(ex, "Kafka producer exception");
    std::cerr << "% Kafka producer exception >>" << errStr << "<<"
                << std::endl;
}

KafkaPublisher::~KafkaPublisher()
{
    // this is being called in the C# dispose method
    this->close();


    if ( conf_ != nullptr )
    { 
        delete conf_;
    }
    if( producer_ != nullptr )
    {
        delete producer_;
    }
}

void KafkaPublisher::close()
{
    if( this->producer_ != nullptr ) { return; }

    std::lock_guard<std::mutex> guard(openLock_);

    if( this->producer_ != nullptr ) { return; }

    delete producer_;
    producer_ = nullptr;
    timer_.stop();
}

void KafkaPublisher::open()
{
    if( this->producer_ != nullptr ) { return; }

    std::lock_guard<std::mutex> guard(openLock_);

    if( this->producer_ != nullptr ) { return; }

    std::string errstr;
    producer_ = RdKafka::Producer::create(conf_, errstr);
    if ( !producer_ ) {
        std::stringstream ss;
        ss << "Failed to create kafka producer (" << errstr << ")";
        throw InvalidOperationException(ss.str());
    }

    // unlike in C# this one is initialized with callback doing nothing for simplicity
    setupKeepAlive_();

    this->timer_.start();
}

void KafkaPublisher::sendInternal(std::shared_ptr<IPackage> package, ProduceDelegate produceDelegate)
{
    /// TODO: CancellationToken

    const auto originalPackage = dynamic_pointer_cast<ByteArrayPackage>(package);

    if( this->producer_ != nullptr ) { return; }
    {
        std::lock_guard<std::mutex> guard(openLock_);
        if( this->producer_ != nullptr )
        { 
            throw InvalidOperationException("Unable to write while producer is closed");
        }
    }

    KafkaPackageExtensions<ByteArrayPackage> kafkaPackage(originalPackage.get());

    bool success = false;
    int tryCount = 0;
    int maxTry = 10;

    {
        std::lock_guard<std::mutex> guard(sendLock_);

        do {

            try {
                tryCount++;

                std::string key;
                bool succ = KafkaPackageExtensions<ByteArrayPackage>( originalPackage.get() ).tryGetKey( key );
                if( !succ ) { continue; }
                auto& data = originalPackage->value();

                produceDelegate(key, data);

                success = true;
            } catch(std::exception ex)
            {
                // TODO

                        // if (tryCount == maxTry) throw;

                        // if (e.Error.Code == ErrorCode.Local_QueueFull)
                        // {
                        //     this.Flush(cancellationToken);
                        // }
                        // else
                        // {
                        //     throw;
                        // }

            }

        } while ( !success && tryCount < maxTry );

    }
    


    // auto& key = originalPackage->

}

void KafkaPublisher::flush( )
{
    /// TODO: CancellationToken

    if ( this->producer_ == nullptr ) { 
        std::stringstream ss;
        ss << "Unable to flush a closed " << typeid(KafkaPublisher).name();
        throw InvalidOperationException( ss.str() );
    }
    auto flushTime = std::chrono::system_clock::now();
    if (flushTime < this->lastFlush_) 
    { 
        return;
    }

    std::lock_guard<std::mutex> guard(flushLock_);

    if (flushTime < this->lastFlush_) 
    { 
        return;
    }

    // TODO: add flush timeout into the configuration
    int flushTimeout = 1000; /// in ms 
    this->producer_->flush( flushTimeout );

    this->lastFlush_ = std::chrono::system_clock::now();
}


void KafkaPublisher::sendKeepAlive()
{

    ProduceDelegate produceKeepAlive = [this](const std::string& key, const ByteArray& message /*, Action<DeliveryReport<string, byte[]>> deliveryHandler, object state*/ )
            {
                // TODO:
                // var topicPartition = (TopicPartition) state;
                // this->producer.produce_(topicPartition, new Message<string, byte[]> { Key = key, Value = message }, deliveryHandler);
            };

    try
    {
        for( auto topicPartition : this->partitionsToKeepAliveWith_ )
        {
            // logger.LogTrace("Sending keep alive msg to {0}", topicPartition);
            //TODO:
            sendInternal( Constants::keepAlivePackage, produceKeepAlive );
            // sendInternal(Constants::keepAlivePackage, produceKeepAlive, state: topicPartition).GetAwaiter().GetResult();
            // logger.LogTrace("Sent keep alive msg to {0}", topicPartition);
        }
    }
    catch(const std::exception& e)
    {
        // logger.LogWarning("Failed to send keep alive msg");    
        std::cerr << "Failed to send keep alive msg " << e.what() << '\n';
    }
    
}
