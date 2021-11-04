#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

    class DeserializingException : public std::runtime_error
    {
    public:
        DeserializingException(const std::string& message) : std::runtime_error(message) {};
    };

} }