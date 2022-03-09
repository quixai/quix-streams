#pragma once

#include <memory>

#include "./eventDefinitions.h"

#include "../../../../transport/codec/ICodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class EventDefinitionsProtobufCodec : public Quix::Transport::ICodec<EventDefinitions>
    {
    public:
        /**
         * Serialize the object with the codec.
         */
        Quix::Transport::ByteArray serialize(const EventDefinitions& obj) const;
        /**
         * Deserialize the byte array with the codec.
         */
        EventDefinitions deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }