#pragma once

#include <string>
#include <functional>

#include "./transportPackageValue.h"
#include "../../io/byteArray.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

class TransportPackageValueCodec
{
public:
    static const uint8_t PROTOCOL_ID_BYTE = 0x01;
    static const uint8_t PROTOCOL_ID_PROTOBUF = 0x02;
    static const uint8_t PROTOCOL_ID_JSON = '{';

    static ByteArray serialize(std::shared_ptr<TransportPackageValue> package);
    static std::shared_ptr<TransportPackageValue> deserialize(const ByteArray& data);
};

} }