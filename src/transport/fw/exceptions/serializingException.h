#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

    class SerializingException : public std::runtime_error
    {
    public:
        SerializingException(const std::string& message) : std::runtime_error(message) {};
    };

} }