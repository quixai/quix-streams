#pragma once

#include <string>
#include <functional>
#include <librdkafka/rdkafkacpp.h>

namespace Quix { namespace Transport {

/**
 * Raised when an error has occured during serialization
 */
class KafkaException : public std::exception
{
    RdKafka::ErrorCode errCode_;

public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    KafkaException(const RdKafka::ErrorCode errCode)
    {

    };

    RdKafka::ErrorCode error() const
    {
        return errCode_;
    }
};

} }