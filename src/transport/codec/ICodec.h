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
class BaseCodec {
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
        BaseCodec(const CodecId& codecId);

    public:

        /**
         * Serialize the object with the codec.
         */
        virtual bool trySerialize(ByteArray& out, const std::shared_ptr<IPackage>& obj) const = 0;

        /**
         * Deserialize the byte array with the codec.
         */
        virtual bool tryDeserialize(std::shared_ptr<IPackage>& out, const std::shared_ptr<ByteArrayPackage> package) const = 0;

        /**
         * Get the unique Identifier of the codec
         */
        const CodecId& codecId() const;

};

template<class T>
class ICodec : public BaseCodec {

        /**
         * Serialize the object with the codec.
         */
        virtual ByteArray serialize(const T& obj) const = 0;
        /**
         * Deserialize the byte array with the codec.
         */
        virtual T deserialize(const ByteArray& package) const = 0;

    public:
        ICodec<T>() : BaseCodec( CodecId( typeid(ICodec<T>).name() ) ) { };

        /**
         * Serialize the object with the codec.
         */
        bool trySerialize(ByteArray& out, const std::shared_ptr<IPackage>& obj) const
        {
            // TODO: add check for package
            out = serialize( std::dynamic_pointer_cast<Package<T>>( obj )->value() );
            return true;
        }
        /**
         * Deserialize the byte array with the codec.
         */
        bool tryDeserialize(std::shared_ptr<IPackage>& out, const std::shared_ptr<ByteArrayPackage> package) const
        {
            auto ret = std::shared_ptr< Package<T> >( new Package<T>( deserialize( package->value() ) ) );
            out = std::dynamic_pointer_cast<Package<T>>( ret );
            return true;
        }
};

} }