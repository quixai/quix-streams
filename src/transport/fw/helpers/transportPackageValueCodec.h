#pragma once

#include <string>
#include <functional>

#include "./transportPackageValue.h"
#include "../../io/byteArray.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

/**
 * Codec used to serialize TransportPackageValue
 * Doesn't inherit from TransportPackageValue because isn't intended for external or generic use
 * and the interface slightly compicates the implementation for no benefit
 */
class TransportPackageValueCodec
{
public:
    /**
     * Definition of the supported protocols ( defined in the first byte of the packet )
     */
    ///Raw byte protocol used by the Quix C# SDK
    static const uint8_t PROTOCOL_ID_BYTE = 0x01;
    ///Protbuf protocol
    static const uint8_t PROTOCOL_ID_PROTOBUF = 0x02;
    ///JSON protocol
    static const uint8_t PROTOCOL_ID_JSON = '{';

    static ByteArray serialize(std::shared_ptr<TransportPackageValue> package);
    static std::shared_ptr<TransportPackageValue> deserialize(const ByteArray& data);
};

} }