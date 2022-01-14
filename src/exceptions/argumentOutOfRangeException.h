#pragma once

#include <string>
#include <functional>

namespace Quix {

/**
 * Raised when program performs invalid operation
 */
class ArgumentOutOfRangeException : public std::exception
{
public:
    /**
     * Initialize with message
     * 
     * @param message message
     */
    ArgumentOutOfRangeException(const std::string& message)
    {

    };
};

}