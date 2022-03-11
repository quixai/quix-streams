#pragma once

#include "./ICodec.h"

namespace Quix { namespace Transport {


/**
 * Simple codec which only copies the binary content of the message 
 */
template<class T>
class BinaryCodec : public ICodec<T>
{
    public:
        /**
         * Serialize the object with the codec.
         */
        ByteArray serialize(const T& value) const
        {
            ByteArray arr = ByteArray::initEmpty( sizeof(T) );

            memcpy(arr.data(), (void*)(&value), sizeof(T) );

            return arr;
        }

        /**
         * Deserialize the byte array with the codec.
         */
        T deserialize(const ByteArray& value) const
        {
            T data;
            memcpy((void*)&data, (void*)value.data(), sizeof(T) );

            return data;
        }

};


} }