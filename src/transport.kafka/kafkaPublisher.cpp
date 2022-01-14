
#include "../exceptions/invalidOperationException.h"
#include "./kafkaPublisher.h"

#include <functional>
#include <sstream>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaPublisher::KafkaPublisher(const PublisherConfiguration& publisherConfiguration, InputTopicConfiguration topicConfiguration)
:
setupKeepAlive_([](){})
{
    conf_ = publisherConfiguration.toProducerConfig();
    std::string errstr;
    producer_ = RdKafka::Producer::create(conf_, errstr);
    if ( !producer_ ) {
        std::stringstream ss;
        ss << "Failed to create kafka producer (" << errstr << ")";
        throw InvalidOperationException(ss.str());
    }

    std::function<void()> hbAction(nullptr);
    auto partition = topicConfiguration.partition();
    if ( partition == Partition::Any )
    {
        hbAction_ = [=](){
            
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
//                   RdKafka::ErrorCode resp =
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
    //   if (resp != RdKafka::ERR_NO_ERROR) {
    //     std::cerr << "% Produce failed: " << RdKafka::err2str(resp)
    //               << std::endl;
    //     delete headers; /* Headers are automatically deleted on produce()
    //                      * success. */
    //   } else {
    //     std::cerr << "% Produced message (" << line.size() << " bytes)"
    //               << std::endl;
    //   }
//            this->producer_->
    };

    if (publisherConfiguration.keepConnectionAlive && publisherConfiguration.keepConnectionAliveInterval > 0)
    {
        setupKeepAlive_ = [this](){ this->hbAction_(); };

        /// TODO: this uses Threading interval so once we rewrite Timer, then this needs to stay in separate thread
        timer_.setAction(std::bind(&KafkaPublisher::sendKeepAlive, this));
        timer_.change(Timer::INFINITE, publisherConfiguration.keepConnectionAliveInterval);
    }

};

KafkaPublisher::~KafkaPublisher()
{
    delete conf_;
    delete producer_;
}
