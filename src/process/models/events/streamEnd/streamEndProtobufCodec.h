#pragma once

#include <memory>

#include "./streamEnd.h"

#include "../../../../transport/codec/ICodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class StreamEndProtobufCodec : public Quix::Transport::ICodec<StreamEnd>
    {
    public:
        /**
         * Serialize the object with the codec.
         */
        Quix::Transport::ByteArray serialize(const StreamEnd& obj) const;
        /**
         * Deserialize the byte array with the codec.
         */
        StreamEnd deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }