#pragma once

#include <mutex>
#include <chrono>
#include <functional>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "../utils/timer.h"

#include "../transport/io/IPublisher.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/byteArray.h"
#include "./IKafkaPublisher.h"
#include "./topicConfiguration.h"
#include "./publisherConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {


// typedef std::function<void(const std::string&, std::shared_ptr<Quix::Transport::IPackage>)> ProduceDelegate;
typedef std::function<RdKafka::ErrorCode(const std::string&, const Quix::Transport::ByteArray&, void* state)> ProduceDelegate;


/**
 * Interface for providing a class a way to push Package to listener
 */
class KafkaPublisher : public IKafkaPublisher {

    std::mutex openLock_;
    std::mutex flushLock_;
    std::mutex sendLock_;

    std::function<void()> hbAction_;
    std::function<void()> setupKeepAlive_;
    CallbackTimer timer_;
    ProduceDelegate produce_;

    bool disposed_ = false;

    std::chrono::time_point<std::chrono::system_clock> lastFlush_ = std::chrono::time_point<std::chrono::system_clock>{};


    std::vector<Partition> partitionsToKeepAliveWith_;    

    RdKafka::Producer*  producer_ = nullptr;
    RdKafka::Conf*      conf_;

    void errorHandler(RdKafka::ErrorCode errorCode) const;
    void sendKeepAlive();
    void sendInternal(std::shared_ptr<Quix::Transport::IPackage>, ProduceDelegate produceDelegate, void* state = nullptr);

public:

    KafkaPublisher(const PublisherConfiguration& publisherConfiguration, InputTopicConfiguration topicConfiguration);

    ~KafkaPublisher();

    /**
     * @brief Close connection to Kafka
     */
    void close();

    /**
     * @brief Flush the queue to Kafka
     */
    void flush();

    /**
     * @brief Open connection to Kafka
     */
    void open();

    /**
     * Send a package
     * 
     * @param package The package to send
     */
    void send(std::shared_ptr<IPackage> package);

};

} } }