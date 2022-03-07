#include <memory>

#include "./parameterDefinitions.h"
#include "./parameterDefinitionsProtobufCodec.h"
#include "./parameterDefinitionsProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

ParameterDefinitionsProtobufCodec::ParameterDefinitionsProtobufCodec() : AbstractCodec( string( typeid(this).name() ) )
{

}


ByteArray ParameterDefinitionsProtobufCodec::serialize(const shared_ptr<IPackage> obj) const
{
    auto package = dynamic_pointer_cast<Package<ParameterDefinitions>>(obj);

    auto& parameterDefinitions = package->value();

    ParameterDefinitionsProto protobufCodec;

    for( auto& parameter : parameterDefinitions.parameters )
    {
        auto protoitem = protobufCodec.add_parameters();

        protoitem->set_id(parameter.id());
        protoitem->set_location(parameter.location());

        if( parameter.name.has_value() )
        {
            protoitem->set_name_value( *(parameter.name) );
        }
        else
        {
            protoitem->set_name_null( true );
        }

        if( parameter.description.has_value() )
        {
            protoitem->set_description_value( *(parameter.description) );
        }
        else
        {
            protoitem->set_description_null( true );
        }

        if( parameter.customProperties.has_value() )
        {
            protoitem->set_customproperties_value( *(parameter.customProperties) );
        }
        else
        {
            protoitem->set_customproperties_null( true );
        }

        if( parameter.format.has_value() )
        {
            protoitem->set_format_value( *(parameter.format) );
        }
        else
        {
            protoitem->set_format_null( true );
        }

        if( parameter.unit.has_value() )
        {
            protoitem->set_unit_value( *(parameter.unit) );
        }
        else
        {
            protoitem->set_unit_null( true );
        }

        if( parameter.maximumValue.has_value() )
        {
            protoitem->set_maximumvalue_value( *(parameter.maximumValue) );
        }
        else
        {
            protoitem->set_maximumvalue_null( true );
        }

        if( parameter.minimumValue.has_value() )
        {
            protoitem->set_minimumvalue_value( *(parameter.minimumValue) );
        }
        else
        {
            protoitem->set_minimumvalue_null( true );
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

const shared_ptr<IPackage> ParameterDefinitionsProtobufCodec::deserialize(const shared_ptr<ByteArrayPackage> package) const
{
    auto& data = package->value();

    ParameterDefinitionsProto protobufCodec;

    //parse from the index 1 and not 0 because index 0 is controlling character to specify codec type
    protobufCodec.ParseFromArray(data.data(), data.len());

    ParameterDefinitions parameterDefinitions;
    parameterDefinitions.parameters.reserve(protobufCodec.parameters_size());

    for( auto& protobufCodec : protobufCodec.parameters() )
    {
        ParameterDefinition parameterDefinition;

        parameterDefinition.setId( protobufCodec.id() );
        parameterDefinition.setLocation( protobufCodec.location() );

        if( protobufCodec.has_name_value() )
        {
            parameterDefinition.name = protobufCodec.name_value();
        }
        if( protobufCodec.has_description_value() )
        {
            parameterDefinition.description = protobufCodec.description_value();
        }
        if( protobufCodec.has_customproperties_value() )
        {
            parameterDefinition.customProperties = protobufCodec.customproperties_value();
        }
        if( protobufCodec.has_maximumvalue_value() )
        {
            parameterDefinition.maximumValue = protobufCodec.maximumvalue_value();
        }
        if( protobufCodec.has_minimumvalue_value() )
        {
            parameterDefinition.minimumValue = protobufCodec.minimumvalue_value();
        }
        if( protobufCodec.has_unit_value() )
        {
            parameterDefinition.unit = protobufCodec.unit_value();
        }
        if( protobufCodec.has_format_value() )
        {
            parameterDefinition.format = protobufCodec.format_value();
        }

        parameterDefinitions.parameters.push_back(
            parameterDefinition
        );
    }

    return 
        std::shared_ptr<IPackage>(
            new Package<ParameterDefinitions>(
                parameterDefinitions
            )
        );
}
