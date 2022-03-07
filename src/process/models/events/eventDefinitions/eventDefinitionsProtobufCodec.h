#pragma once

#include <memory>

#include "../../../../transport/codec/abstractCodec.h"
#include "../../../../transport/io/package.h"

namespace Quix { namespace Process {

    class EventDefinitionsProtobufCodec : public Quix::Transport::AbstractCodec
    {
    public:
        EventDefinitionsProtobufCodec();
        /**
         *  Serialize object into the ByteArray
         */
        Quix::Transport::ByteArray serialize(const std::shared_ptr<Quix::Transport::IPackage> obj) const;
        /**
         *  Deserialize object from the ByteArray
         */
        const std::shared_ptr<Quix::Transport::IPackage> deserialize(const std::shared_ptr<Quix::Transport::ByteArrayPackage> package) const;
    };
} }