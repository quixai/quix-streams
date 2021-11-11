#include <string>
#include <functional>

#include <iostream>

#include "./byteCodec.h"
#include "../../registry/codecRegistry.h"

using namespace Quix::Transport;
using namespace std;

ByteCodec::ByteCodec(): AbstractCodec(CodecId("ByteCodec")){};

ByteArray ByteCodec::serialize(const std::shared_ptr<IPackage> obj) const
{
    //returns only the casted previous value since the obj variable should already be of byte array type
    return dynamic_pointer_cast<ByteArrayPackage>(obj)->value();
}

const std::shared_ptr<IPackage> ByteCodec::deserialize(const std::shared_ptr<ByteArrayPackage> package) const
{
    //returns only the casted previous value since the package variable is being parent of IPackage
    return dynamic_pointer_cast<IPackage>(package);
}