#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

/**
 * Raised when an error has occured during serialization
 */
class SerializingException : public std::exception
{
public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    SerializingException(const std::string& message)
    {

    };
};

} }