#pragma once

#include <string>
#include <functional>

#include "../../io/IPackage.h"
#include "../../codec/ICodec.h"
#include "../../codec/codecId.h"
#include "../../io/IPublisher.h"
#include "../../io/ISubscriber.h"
#include "../../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    /**
     *  Codec for serializing and deserializing a ByteArray
     */
    class ByteCodec : public ICodec< ByteArray >{
        public:
            ByteCodec();
            /**
             *  Serialize object into the ByteArray
             */
            ByteArray serialize( const ByteArray& obj ) const;
            /**
             *  Deserialize object from the ByteArray
             */
            ByteArray deserialize( const ByteArray& package ) const;
    };

} }