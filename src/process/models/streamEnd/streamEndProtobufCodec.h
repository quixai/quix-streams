#pragma once

#include <memory>

#include "./streamEnd.h"

#include "../../../transport/codec/ICodec.h"
#include "../../../transport/io/package.h"

namespace Quix { namespace Process {

    class StreamEndProtobufCodec : public Quix::Transport::ICodec<StreamEnd>
    {
    public:
        /**
         *  Serialize object into the ByteArray
         */
        Quix::Transport::ByteArray serialize(const StreamEnd& obj) const;
        /**
         *  Deserialize object from the ByteArray
         */
        StreamEnd deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }