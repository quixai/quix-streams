#pragma once

#include <memory>

#include "./parameterDefinitions.h"

#include "../../../../transport/codec/ICodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class ParameterDefinitionsProtobufCodec : public Quix::Transport::ICodec<ParameterDefinitions>
    {
    public:
        /**
         *  Serialize object into the ByteArray
         */
        Quix::Transport::ByteArray serialize(const ParameterDefinitions& obj) const;
        /**
         *  Deserialize object from the ByteArray
         */
        ParameterDefinitions deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }