#pragma once

#include <string>
#include <functional>

#include "../../io/byteArray.h"
#include "../codecs/codecBundle.h"
#include "../../io/package.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

/**
 * Describes how to de/serialize the IPackage.value()
 */
class TransportPackageValue
{
    std::shared_ptr<RawBytePackage> value_;
    CodecBundle codecBundle_;

public:
    /**
     * Initializes a new instance TransportPackageValue
     * 
     * @param value The value to de/serialize
     * @param codecBundle The codec details to use for de/serialization
     */
    TransportPackageValue(
        std::shared_ptr<RawBytePackage> value, 
        const CodecBundle& codecBundle
    )
     : 
     value_(value), 
     codecBundle_(codecBundle)
    {

    };

    /**
     * copy constructor
     */
    TransportPackageValue(const TransportPackageValue& other) = default;

    /**
     * The value to de/serialize
     */
    std::shared_ptr<RawBytePackage> value() const 
    {
        return value_;
    }

    /**
     * The metadata that belongs to the value
     */
    const CodecBundle& codecBundle() const
    {
        return codecBundle_;
    }

    /**
     * Compare with other TransportPackageValue
     * 
     * @param other TransportPackageValue to compare with
     */
    bool operator==(const TransportPackageValue& other) const
    {
        return 
            codecBundle_ == other.codecBundle()
                &&
            *value_ == *(other.value())
            ;
    }

};

} }