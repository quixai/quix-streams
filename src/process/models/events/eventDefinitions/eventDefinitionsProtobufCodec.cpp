#include <memory>

#include "./eventDefinitions.h"
#include "./eventDefinitionsProtobufCodec.h"
#include "./eventDefinitionsProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

EventDefinitionsProtobufCodec::EventDefinitionsProtobufCodec() : AbstractCodec( string( typeid(this).name() ) )
{

}


ByteArray EventDefinitionsProtobufCodec::serialize(const shared_ptr<IPackage> obj) const
{
    auto package = dynamic_pointer_cast<Package<EventDefinitions>>(obj);

    auto& eventDefinitions = package->value();

    EventDefinitionsProto protobufCodec;

    for( auto& event : eventDefinitions.events )
    {
        auto protoitem = protobufCodec.add_events();

        protoitem->set_id(event.id());
        protoitem->set_level(event.level);
        protoitem->set_location(event.location());

        if( event.name.has_value() )
        {
            protoitem->set_name_value( *(event.name) );
        }
        else
        {
            protoitem->set_name_null( true );
        }

        if( event.description.has_value() )
        {
            protoitem->set_description_value( *(event.description) );
        }
        else
        {
            protoitem->set_description_null( true );
        }

        if( event.customProperties.has_value() )
        {
            protoitem->set_customproperties_value( *(event.customProperties) );
        }
        else
        {
            protoitem->set_customproperties_null( true );
        }

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

const shared_ptr<IPackage> EventDefinitionsProtobufCodec::deserialize(const shared_ptr<ByteArrayPackage> package) const
{
    auto& data = package->value();

    EventDefinitionsProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    EventDefinitions eventDefinitions;
    eventDefinitions.events.reserve(protobufCodec.events_size());

    for( auto& protobufCodec : protobufCodec.events() )
    {
        EventDefinition eventDefinition;

        eventDefinition.setId( protobufCodec.id() );
        eventDefinition.level = (EventDefinition::EventLevel)(protobufCodec.level());
        eventDefinition.setLocation( protobufCodec.location() );

        if( protobufCodec.has_name_value() )
        {
            eventDefinition.name = protobufCodec.name_value();
        }
        if( protobufCodec.has_description_value() )
        {
            eventDefinition.description = protobufCodec.description_value();
        }
        if( protobufCodec.has_customproperties_value() )
        {
            eventDefinition.customProperties = protobufCodec.customproperties_value();
        }

        eventDefinitions.events.push_back(
            eventDefinition
        );
    }

    return 
        std::shared_ptr<IPackage>(
            new Package<EventDefinitions>(
                eventDefinitions
            )
        );
}
