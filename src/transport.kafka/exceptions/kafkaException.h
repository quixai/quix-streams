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

public:

    const RdKafka::ErrorCode errCode;
    const std::string message;

    /**
     * Initialize with message
     * 
     * @param message message
     */
    KafkaException(const RdKafka::ErrorCode errCode, const std::string& message = "")
    :
    errCode(errCode),
    message(message)
    {

    };
};

} }