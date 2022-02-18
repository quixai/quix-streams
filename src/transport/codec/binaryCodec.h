#pragma once

#include "./abstractCodec.h"

namespace Quix { namespace Transport {


/**
 * Simple codec which only copies the binary content of the message 
 */
template<class T>
class BinaryCodec : public AbstractCodec
{
    private:
    public:
        BinaryCodec(): AbstractCodec( std::string( typeid(this).name() ) )
        {

        };

        ByteArray serialize(const std::shared_ptr< IPackage > obj) const
        {
            auto casted = std::dynamic_pointer_cast< Package<T> >(obj);

            //returns only the casted previous value since the obj variable should already be of byte array type
            ByteArray arr = ByteArray::initEmpty( sizeof(T) );

            T value = casted->value();
            memcpy(arr.data(), (void*)(&value), sizeof(T) );

            return arr;
        };

        const std::shared_ptr<IPackage> deserialize(const std::shared_ptr<ByteArrayPackage> package) const
        {
            //returns only the casted previous value since the package variable is being parent of IPackage

            T data;
            memcpy((void*)&data, (void*)package->value().data(), sizeof(T) );

            return std::shared_ptr<IPackage>( new Package<T>(package, data) );
        };
};


} }