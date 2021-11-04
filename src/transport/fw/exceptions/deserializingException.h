#pragma once

#include <string>
#include <functional>

namespace Quix { namespace Transport {

    class DeserializingException : public std::exception
    {
    public:
        DeserializingException(const std::string& message){};
    };

} }