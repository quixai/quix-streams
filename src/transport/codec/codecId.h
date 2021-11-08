#pragma once

#include <string>

namespace Quix { namespace Transport {

class CodecId : public std::string {
public:
    inline CodecId(const std::string& codecId = "UNKNOWN"):std::string(codecId){};
    inline CodecId(const CodecId& codecId) = default;
};

} }