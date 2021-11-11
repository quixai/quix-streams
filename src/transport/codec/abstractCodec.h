#pragma once

#include "../io/IPackage.h"

#include <string>
#include <memory>
#include <memory>
#include <vector>
#include <unordered_map>


#include "./codecId.h"
#include "../io/IPackage.h"
#include "../io/package.h"


namespace Quix { namespace Transport {

/**
 * Abstract defitiion ( interface ) of a Codec capable of serializing and deserializing package
 */
class AbstractCodec {
    private:
        /**
         * The unique Identifier of the codec. Useful to differentiate different codecs for the same type when there are
         *  multiple defined.
         *  For example, if a type has both JSON and Protobuf, the Ids would be 'JSON' and 'Protobuf'.
         *  Note, you can, but do not need to use the type's full name. For serialization purposes it is better to use as short
         *  as possible yet unique Ids.
         */
        const CodecId codecId_;

    protected:
        /**
         * Initialize Codec
         * 
         * @param codecId CodecId unique identifier of codec.
         */
        AbstractCodec(const CodecId& codecId);

    public:
        /**
         * Get the unique Identifier of the codec
         */
        const CodecId& codecId() const;
        /**
         * Serialize the object with the codec.
         */
        virtual ByteArray serialize(const std::shared_ptr<IPackage> obj) const = 0;
        /**
         * Deserialize the byte array with the codec.
         */
        virtual const std::shared_ptr<IPackage> deserialize(const std::shared_ptr<RawBytePackage> package) const = 0;

};

} }