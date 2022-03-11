#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

/**
 * Raised when an value is out of allowed range
 */
class OutOfRangeException : public std::exception
{
public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    OutOfRangeException(const std::string& message)
    {

    };
};

} }