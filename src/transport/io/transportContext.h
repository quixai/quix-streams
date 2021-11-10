#pragma once

#include <map>
#include <string>

namespace Quix { namespace Transport {

class TransportContext : public std::map<std::string, std::string>{
public:
    inline TransportContext() = default;
    inline TransportContext(const std::map<std::string, std::string>& input) : std::map<std::string, std::string>(input) {};
};

} }