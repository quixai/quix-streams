#pragma once

#include <string>
#include <functional>
#define __STDC_WANT_LIB_EXT1__ 1
#include <rdkafkacpp.h>

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

    std::string kafkaMessage() const{
        return RdKafka::err2str( this->errCode );
    }
};

} }