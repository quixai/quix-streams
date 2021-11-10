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

    ByteArray TransportPackageValueCodecProtobuf::serialize(std::shared_ptr<TransportPackageValue> package)
    {
        auto packageValue = package->value()->value();
        auto packageMetadata = package->value()->metaData();

        TransportPackageValueCodecProtobufRaw protobufCodec;
        //set codecId
        protobufCodec.set_codecid(package->codecBundle().codecId().c_str());
        //set modelKey
        protobufCodec.set_modelkey(package->codecBundle().modelKey().key().c_str());
        //set data array
        protobufCodec.set_data(std::string((const char*)packageValue.data(), packageValue.len()));
        auto revc = protobufCodec.data().size();
        //set metaData map
        auto& metadata = *protobufCodec.mutable_metadata();
        for (auto it = packageMetadata.begin(); it != packageMetadata.end(); it++)
        {
            metadata[it->first] = it->second;
        }

        //serialize into array buffer
        size_t size = protobufCodec.ByteSizeLong();

        //initialize return value
        auto bytePackageValue = ByteArray::initEmpty(size+1);
        auto array = bytePackageValue.data();
        array[0] = TransportPackageValueCodec::PROTOCOL_ID_PROTOBUF;
        protobufCodec.SerializeToArray((uint8_t*)array+1, size);

        //return object containing raw array and its length
        return bytePackageValue;
    }

    std::shared_ptr<TransportPackageValue> TransportPackageValueCodecProtobuf::deserialize(const ByteArray& data)
    {
        TransportPackageValueCodecProtobufRaw protobufCodec;

        //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
        protobufCodec.ParseFromArray(data.data()+1, data.len()-1);

        //TODO: set codecId

        auto& protobufMetadata = protobufCodec.metadata();
        MetaData metadata;
        for (auto it = protobufMetadata.begin(); it != protobufMetadata.end(); it++)
        {
            metadata[it->first] = it->second;
        }

        return 
            std::shared_ptr<TransportPackageValue>(
                new TransportPackageValue(
                    std::shared_ptr<RawBytePackage>(
                        new RawBytePackage(
                            ByteArray(protobufCodec.data()),
                            protobufCodec.modelkey(),
                            metadata)
                    ),
                    CodecBundle(
                        ModelKey(protobufCodec.modelkey()), 
                        CodecId(protobufCodec.codecid())
                    )
                )
            );                
    }

} }