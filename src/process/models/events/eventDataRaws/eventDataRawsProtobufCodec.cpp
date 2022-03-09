#include <memory>

#include "./eventDataRaws.h"
#include "./eventDataRawsProtobufCodec.h"
#include "./eventDataRawsProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;


ByteArray EventDataRawProtobufCodec::serialize(const EventDataRaws& eventDataRaws) const
{
    EventDataRawsProto protobufCodec;

    for( auto& event : eventDataRaws.events )
    {
        auto protoitem = protobufCodec.add_events();

        protoitem->set_timestamp(event.timestamp);

        auto prototags = protoitem->mutable_tags();
        for( auto &it : event.tags )
        {
            (*prototags)[it.first] = it.second;
        }
        protoitem->set_id(event.id);
        protoitem->set_value(event.value);
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

EventDataRaws EventDataRawProtobufCodec::deserialize(const ByteArray& data) const
{
    EventDataRawsProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    EventDataRaws eventDataRaws;
    eventDataRaws.events.reserve(protobufCodec.events_size());

    for( auto& protobufCodec : protobufCodec.events() )
    {
        EventDataRaw eventDataRaw;

        eventDataRaw.id = protobufCodec.id();
        for( auto &it : protobufCodec.tags() )
        {
            eventDataRaw.tags[it.first] = it.second;
        }
        eventDataRaw.id = protobufCodec.id();
        eventDataRaw.value = protobufCodec.value();
        eventDataRaw.timestamp = protobufCodec.timestamp();

        eventDataRaws.events.push_back(
            eventDataRaw
        );
    }

    // streamEnd.streamEndType = (StreamEnd::StreamEndType)protobufCodec.streamendtype();


    return  eventDataRaws;
}
