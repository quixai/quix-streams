#pragma once

#include "../transport/io/IPublisher.h"

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 */
class IKafkaPublisher : public Quix::Transport::IPublisher{

public:

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