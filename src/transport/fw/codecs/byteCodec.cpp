#include <string>
#include <functional>

#include <iostream>

#include "./byteCodec.h"
#include "../../registry/codecRegistry.h"

using namespace Quix::Transport;
using namespace std;

ByteCodec::ByteCodec() { };

ByteArray ByteCodec::serialize(const ByteArray& obj) const
{
    //returns only the casted previous value since the obj variable should already be of byte array type
    return obj;
}

ByteArray ByteCodec::deserialize(const ByteArray& package) const
{
    //returns only the casted previous value since the package variable is being parent of IPackage
    return package;
}