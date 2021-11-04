#pragma once

#include <string>
#include <functional>

#include "../../io/rawBytePackageValue.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

class TransportPackageValueCodecProtobuf
{
public:
    static RawBytePackageValue Serialize(RawBytePackage* package);
    static RawBytePackage* Deserialize(const RawBytePackageValue& data);
};

class TransportPackageValueCodec
{
public:
    static const uint8_t PROTOCOL_ID_BYTE = 0x01;
    static const uint8_t PROTOCOL_ID_PROTOBUF = 0x02;
    static const uint8_t PROTOCOL_ID_JSON = '{';

    static RawBytePackageValue Serialize(RawBytePackage* package);
    static RawBytePackage* Deserialize(const RawBytePackageValue& data);
};

} }