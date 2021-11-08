#pragma once

#include <string>
#include <functional>

#include "../../io/rawBytePackageValue.h"
#include "../codecs/codecBundle.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

class TransportPackageValue
{
    std::shared_ptr<RawBytePackage> value_;
    CodecBundle codecBundle_;
public:
    TransportPackageValue(std::shared_ptr<RawBytePackage> value, const CodecBundle& codecBundle) : value_(value), codecBundle_(codecBundle) {};
    TransportPackageValue(const TransportPackageValue& other) = default;
    std::shared_ptr<RawBytePackage> value() const {
        return value_;
    }
    const CodecBundle& codecBundle() const {
        return codecBundle_;
    }
    bool operator==(const TransportPackageValue& other) const {
        return 
            codecBundle_ == other.codecBundle()
                &&
            *value_ == *(other.value())
            ;
    }
};

} }