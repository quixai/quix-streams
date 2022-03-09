#pragma once

#include <memory>

#include "./parameterDataRaw.h"

#include "../../../../transport/codec/ICodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class ParameterDataRawProtobufCodec : public Quix::Transport::ICodec<ParameterDataRaw>
    {
    public:
        /**
         * Serialize the object with the codec.
         */
        Quix::Transport::ByteArray serialize(const ParameterDataRaw& obj) const;
        /**
         * Deserialize the byte array with the codec.
         */
        ParameterDataRaw deserialize(const Quix::Transport::ByteArray& package) const;
    };

} }