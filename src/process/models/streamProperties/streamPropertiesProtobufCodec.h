#pragma once

#include <memory>

#include "./streamProperties.h"

#include "../../../transport/codec/ICodec.h"
#include "../../../transport/io/package.h"

namespace Quix { namespace Process {

    class StreamPropertiesProtobufCodec : public Quix::Transport::ICodec<StreamProperties>
    {
    public:
        /**
         * Serialize the object with the codec.
         */
        Quix::Transport::ByteArray serialize(const StreamProperties& obj) const;
        /**
         * Deserialize the byte array with the codec.
         */
        StreamProperties deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }