#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

/**
 * Raised when an error has occured during deserialization
 */
class DeserializingException : public std::exception
{
public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    DeserializingException(const std::string& message)
    {

    };
};

} }