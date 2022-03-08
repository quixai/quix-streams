#include <memory>

#include "./streamEnd.h"
#include "./streamEndProtobufCodec.h"
#include "./streamEndProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

StreamEndProtobufCodec::StreamEndProtobufCodec() : AbstractCodec( string( typeid(this).name() ) )
{

}


ByteArray StreamEndProtobufCodec::serialize(const shared_ptr<IPackage> obj) const
{
    auto package = dynamic_pointer_cast<Package<StreamEnd>>(obj);

    StreamEndProto protobufCodec;

    protobufCodec.set_streamendtype(package->value().streamEndType);





    //serialize into array buffer
    size_t size = protobufCodec.ByteSizeLong();

    //initialize return value
    auto bytePackageValue = ByteArray::initEmpty(size);
    auto array = bytePackageValue.data();

    protobufCodec.SerializeToArray((uint8_t*)array, size);

    //return object containing raw array and its length
    return bytePackageValue;    
}

const shared_ptr<IPackage> StreamEndProtobufCodec::deserialize(const shared_ptr<ByteArrayPackage> package) const
{
    auto& data = package->value();

    StreamEndProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    StreamEnd streamEnd;
    streamEnd.streamEndType = (StreamEnd::StreamEndType)protobufCodec.streamendtype();


    return 
        std::shared_ptr<IPackage>(
            new Package<StreamEnd>(
                streamEnd
            )
        );
}
