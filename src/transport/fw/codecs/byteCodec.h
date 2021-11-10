#pragma once

#include <string>
#include <functional>

#include "../../io/IPackage.h"
#include "../../codec/abstractCodec.h"
#include "../../codec/codecId.h"
#include "../../io/IPublisher.h"
#include "../../io/ISubscriber.h"
#include "../../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    /**
     *  Codec for serializing and deserializing a ByteArray
     */
    class ByteCodec : public AbstractCodec{
        public:
            ByteCodec();
            /**
             *  Static initializer used for the registering codec into the CodecRegistry
             */
            static void initialize();
            /**
             *  Serialize object into the ByteArray
             */
            virtual ByteArray serialize(const std::shared_ptr<IPackage> obj) const;
            /**
             *  Deserialize object from the ByteArray
             */
            virtual const std::shared_ptr<IPackage> deserialize(const std::shared_ptr<RawBytePackage> package) const;
    };

} }