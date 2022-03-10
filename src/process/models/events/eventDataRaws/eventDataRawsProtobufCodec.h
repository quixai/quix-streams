#pragma once

#include <memory>

#include "./eventDataRaws.h"

#include "../../../../transport/codec/ICodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class EventDataRawProtobufCodec : public Quix::Transport::ICodec<EventDataRaws>
    {
    public:
        /**
         *  Serialize object into the ByteArray
         */
        Quix::Transport::ByteArray serialize(const EventDataRaws& obj) const;
        /**
         *  Deserialize object from the ByteArray
         */
        EventDataRaws deserialize(const Quix::Transport::ByteArray& package) const;
    };
} }