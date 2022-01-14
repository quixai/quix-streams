#pragma once

#include <librdkafka/rdkafkacpp.h>

#include "../utils/timer.h"

#include "../transport/io/IPublisher.h"
#include "../transport/io/byteArray.h"
#include "./IKafkaPublisher.h"
#include "./topicConfiguration.h"
#include "./publisherConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 */
class KafkaPublisher : public IKafkaPublisher, public Quix::Transport::IPublisher{

    std::function<void()> hbAction_;
    std::function<void()> setupKeepAlive_;
    CallbackTimer timer_;
    std::function<void(const std::string& topicName, const Quix::Transport::ByteArray& data)> produce_;

    std::vector<Partition> partitionsToKeepAliveWith_;    

    RdKafka::Producer*  producer_;
    RdKafka::Conf*      conf_;

    void sendKeepAlive();

public:

    KafkaPublisher(const PublisherConfiguration& publisherConfiguration, InputTopicConfiguration topicConfiguration);

    ~KafkaPublisher();

    /**
     * @brief Close connection to Kafka
     */
    virtual void close() = 0;

    /**
     * @brief Flush the queue to Kafka
     */
    virtual void flush() = 0;

    /**
     * @brief Open connection to Kafka
     */
    virtual void open() = 0;

};

} } }