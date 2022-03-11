#pragma once

#include <string>
#include <functional>

namespace Quix {

/**
 * Raised when program performs invalid operation
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

}