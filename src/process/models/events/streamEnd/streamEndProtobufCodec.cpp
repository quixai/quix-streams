#include <memory>

#include "./streamEnd.h"
#include "./streamEndProtobufCodec.h"
#include "./streamEndProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;



ByteArray StreamEndProtobufCodec::serialize(const StreamEnd& value) const
{
    StreamEndProto protobufCodec;

    protobufCodec.set_streamendtype(value.streamEndType);





    //serialize into array buffer
    size_t size = protobufCodec.ByteSizeLong();

    //initialize return value
    auto bytePackageValue = ByteArray::initEmpty(size);
    auto array = bytePackageValue.data();

    protobufCodec.SerializeToArray((uint8_t*)array, size);

    //return object containing raw array and its length
    return bytePackageValue;    
}

StreamEnd StreamEndProtobufCodec::deserialize(const ByteArray& data) const
{
    StreamEndProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    StreamEnd streamEnd;
    streamEnd.streamEndType = (StreamEnd::StreamEndType)protobufCodec.streamendtype();


    return streamEnd;
}
