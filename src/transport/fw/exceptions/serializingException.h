#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

    class SerializingException : public std::exception
    {
    public:
        SerializingException(const std::string& message) {};
    };

} }