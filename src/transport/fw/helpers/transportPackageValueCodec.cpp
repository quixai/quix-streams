#include <cstring>
#include <sstream>
#include <memory>
#include <string>

#include "../exceptions/deserializingException.h"

#include "../../codec/codecId.h"
#include "./transportPackageValue.h"
#include "./transportPackageValueCodec.h"
#include "./transportPackageValueCodecProtobuf.h"
#include "./transportPackageValueCodecProtobufRaw.pb.h"

namespace Quix { namespace Transport {

    ByteArray TransportPackageValueCodec::serialize(std::shared_ptr<TransportPackageValue> package)
    {
        return TransportPackageValueCodecProtobuf::serialize(package);
    }

    std::shared_ptr<TransportPackageValue> TransportPackageValueCodec::deserialize(const ByteArray& data)
    {
        if(data.len() < 1)
        {
            throw DeserializingException("Failed to Deserialize TransportPackageValueCodec. Recieved array must contain at least one byte.");
        }
        switch(data.begin()[0])
        {
            case TransportPackageValueCodec::PROTOCOL_ID_PROTOBUF:
                //Protobuf codec
                return TransportPackageValueCodecProtobuf::deserialize(data);
                break;                    
            case TransportPackageValueCodec::PROTOCOL_ID_BYTE:
            case TransportPackageValueCodec::PROTOCOL_ID_JSON:
                //TODO: implement backwards Csharp compatibility
            default:
                std::stringstream ss;
                ss << "Failed to Deserialize TransportPackageValueCodec. Unknown codec '" << data.begin()[0] << "'.";
                throw DeserializingException(ss.str());
        };
    }

} }