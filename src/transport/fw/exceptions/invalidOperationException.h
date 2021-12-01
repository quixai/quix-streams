#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

/**
 * Raised when an error has occured during deserialization
 */
class InvalidOperationException : public std::exception
{
public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    InvalidOperationException(const std::string& message)
    {

    };
};

} }