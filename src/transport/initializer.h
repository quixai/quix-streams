#pragma once

#include "./fw/codecs/byteCodec.h"
#include "./initializer.h"

namespace Quix { namespace Transport {

static int initialize(){
    ByteCodec::initialize();
    return 0;
};

} }
