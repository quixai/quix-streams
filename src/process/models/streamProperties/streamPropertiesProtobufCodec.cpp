#include <memory>

#include "./streamProperties.h"
#include "./streamPropertiesProtobufCodec.h"
#include "./streamPropertiesProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

StreamPropertiesProtobufCodec::StreamPropertiesProtobufCodec() : AbstractCodec( string( typeid(this).name() ) )
{

}


ByteArray StreamPropertiesProtobufCodec::serialize(const shared_ptr<IPackage> obj) const
{
    auto package = dynamic_pointer_cast<Package<StreamProperties>>(obj);
    auto& value = package->value();

    StreamPropertiesProto protobufCodec;

    protobufCodec.set_name(value.name);
    protobufCodec.set_location(value.location);

    auto protobufCodecMetadata = protobufCodec.mutable_metadata();
    for( auto it = value.metadata.begin(); it != value.metadata.end(); it++ )
    {
        (*protobufCodecMetadata)[it->first] = it->second;
    }

    for( auto& val : value.parents )
    {
        protobufCodec.add_parents( val );
    }

    if( value.timeOfRecording.has_value() )
    {
        auto nanos = std::chrono::duration_cast<std::chrono::milliseconds>((*(value.timeOfRecording)).time_since_epoch()).count();
        protobufCodec.set_timeofrecording_value( nanos );
    }
    else
    {
        protobufCodec.set_timeofrecording_null( true );
    }




    //serialize into array buffer
    size_t size = protobufCodec.ByteSizeLong();

    //initialize return value
    auto bytePackageValue = ByteArray::initEmpty(size);
    auto array = bytePackageValue.data();

    protobufCodec.SerializeToArray((uint8_t*)array, size);

    //return object containing raw array and its length
    return bytePackageValue;    
}

const shared_ptr<IPackage> StreamPropertiesProtobufCodec::deserialize(const shared_ptr<ByteArrayPackage> package) const
{
    auto& data = package->value();

    StreamPropertiesProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    StreamProperties streamProperties;

    for( auto it = protobufCodec.metadata().begin(); it != protobufCodec.metadata().end(); it++ )
    {
        streamProperties.metadata[it->first] = it->second;
    }

    streamProperties.location = protobufCodec.location();
    streamProperties.name = protobufCodec.name();
    streamProperties.parents = std::vector<std::string>( protobufCodec.parents().begin(), protobufCodec.parents().end() );

    if( protobufCodec.has_timeofrecording_value() )
    {
        auto duration = protobufCodec.timeofrecording_value();
        std::chrono::milliseconds dur(duration);
        std::chrono::time_point<std::chrono::system_clock> dt(dur);
        streamProperties.timeOfRecording = dt;
    }


    return 
        std::shared_ptr<IPackage>(
            new Package<StreamProperties>(
                streamProperties
            )
        );
}
