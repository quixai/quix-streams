#pragma once

#include <string>
#include <functional>

#include "./transportPackageValue.h"
#include "../../io/byteArray.h"
#include "../../io/IPackage.h"

namespace Quix { namespace Transport {

class TransportPackageValueCodecProtobuf
{
public:
    static ByteArray serialize(std::shared_ptr<TransportPackageValue> package);
    static std::shared_ptr<TransportPackageValue> deserialize(const ByteArray& data);
};

} }