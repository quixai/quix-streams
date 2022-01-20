#pragma once

#include <vector>

#include "../utils/eventHandler.h"
#include "./exceptions/kafkaException.h"
#include "./topicConfiguration.h"
#include "./subscriberConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 */
class IKafkaSubscriber{

public:

    /**
     * Raised when <see cref="Exception"/> occurred.
     * 
     * Kafka exceptions are raised as <see cref="KafkaException"/>. See <see cref="KafkaException.Error"/> for exception details.
     */
    EventHandler<KafkaException> errorOccurred;

    /**
     * @brief Close connection to Kafka
     */
    virtual void close() = 0;

    /**
     * @brief Commits all offsets for the current topic partition assignments
     */
    virtual void commitOffsets() = 0;

    /**
     * @brief Commit a list of offsets limited by the topics this output had previously subscribed to
     * 
     * @param offsets The offsets to commit
     */
    virtual void commitOffsets(const std::vector<TopicPartitionOffset>& offsets) = 0;

    /**
     * @brief Open connection to Kafka
     */
    virtual void open() = 0;

};

} } }