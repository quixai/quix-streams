#pragma once

#include <mutex>
#include <chrono>
#include <functional>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "../utils/timer.h"

#include "../transport/io/ISubscriber.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/byteArray.h"
#include "./subscriberConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {


// typedef std::function<void(const std::string&, std::shared_ptr<Quix::Transport::IPackage>)> ProduceDelegate;
typedef std::function<void(const std::string&, const ByteArray&, void* state)> ProduceDelegate;


/**
 * Interface for providing a class a way to push Package to listener
 */
class KafkaSubscriber : public Quix::Transport::ISubscriber{

    std::mutex consumerLock_;
    std::mutex workerThreadLock_;

    const SubscriberConfiguration configuration;

public:

    KafkaSubscriber( const SubscriberConfiguration& configuration );

    ~KafkaSubscriber();

};

} } }