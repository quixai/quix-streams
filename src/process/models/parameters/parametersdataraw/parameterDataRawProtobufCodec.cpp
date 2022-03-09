#include <memory>

#include "./parameterDataRaw.h"
#include "./parameterDataRawProtobufCodec.h"
#include "./parameterDataRawProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

template<typename ProtobufT, typename ValueIt>
void addElementToProtobufItem(ProtobufT* val, ValueIt it)
{
    val->set_header(it->first);

    for( auto& value : it->second.values )
    {
        val->add_values(value);
    }
    for( const auto& isnull : it->second.isnull )
    {
        val->add_isnull(isnull);
    }
}

ByteArray ParameterDataRawProtobufCodec::serialize(const ParameterDataRaw& parameterDataRaw) const
{
    ParameterDataRawProto protobufCodec;

    protobufCodec.set_epoch(parameterDataRaw.epoch);

    // add timestamps
    for( vector<long long>::const_iterator it = parameterDataRaw.timestamps.cbegin() ; it != parameterDataRaw.timestamps.cend() ; ++it )
    {
        protobufCodec.add_timestamps(*it);
    }

    // add strings
    for( map<string, ParameterDataRaw::OptionalVector<string>>::const_iterator it = parameterDataRaw.stringValues.cbegin() ; it != parameterDataRaw.stringValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_string_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_StringValues, map<string, ParameterDataRaw::OptionalVector<string>>::const_iterator>( val, it );
    }
    // add tags
    for( map<string, ParameterDataRaw::OptionalVector<string>>::const_iterator it = parameterDataRaw.tagValues.cbegin() ; it != parameterDataRaw.tagValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_tag_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_StringValues, map<string, ParameterDataRaw::OptionalVector<string>>::const_iterator>( val, it );
    }
    // add numeric values
    for( map<string, ParameterDataRaw::OptionalVector<double>>::const_iterator it = parameterDataRaw.numericValues.cbegin() ; it != parameterDataRaw.numericValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_numeric_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_NumericValues, map<string, ParameterDataRaw::OptionalVector<double>>::const_iterator>( val, it );
    }
    // add binary values
    for( map<string, ParameterDataRaw::OptionalVector<ByteArray>>::const_iterator it = parameterDataRaw.binaryValues.cbegin() ; it != parameterDataRaw.binaryValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_binary_values();

        val->set_header(it->first);

        for( auto& value : it->second.values )
        {
            val->add_values(value.data(), value.len());
        }
        for( const auto& isnull : it->second.isnull )
        {
            val->add_isnull(isnull);
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

ParameterDataRaw ParameterDataRawProtobufCodec::deserialize(const ByteArray& data) const
{
    ParameterDataRawProto protobufCodec;

    //parse protobuf object from the input byte array
    protobufCodec.ParseFromArray(data.data(), data.len());

    //load timestamps
    vector<long long> timestamps(protobufCodec.timestamps().begin(), protobufCodec.timestamps().end());

    //load string values
    map<string, ParameterDataRaw::OptionalVector<string>> stringValues;
    for( auto it = protobufCodec.string_values().begin(); it != protobufCodec.string_values().end() ; it++ )
    {
        stringValues.emplace(
            it->header(), 
            ParameterDataRaw::OptionalVector<string>( 
                vector<string>(it->values().begin(), it->values().end()),
                vector<bool>(it->isnull().begin(), it->isnull().end())
            )
        );
    }

    //load string values
    map<string, ParameterDataRaw::OptionalVector<string>> tagValues;
    for( auto it = protobufCodec.tag_values().begin(); it != protobufCodec.tag_values().end() ; it++ )
    {
        tagValues.emplace(
            it->header(), 
            ParameterDataRaw::OptionalVector<string>( 
                vector<string>(it->values().begin(), it->values().end()),
                vector<bool>(it->isnull().begin(), it->isnull().end())
            )
        );
    }

    //load numeric values
    map<string, ParameterDataRaw::OptionalVector<double>> numericValues;
    for( auto it = protobufCodec.numeric_values().begin(); it != protobufCodec.numeric_values().end() ; it++ )
    {
        numericValues.emplace(
            it->header(), 
            ParameterDataRaw::OptionalVector<double>( 
                vector<double>(it->values().begin(), it->values().end()),
                vector<bool>(it->isnull().begin(), it->isnull().end())
            )
        );
    }

    map<string, ParameterDataRaw::OptionalVector<ByteArray>> binaryValues;
    for( auto it = protobufCodec.binary_values().begin(); it != protobufCodec.binary_values().end() ; it++ )
    {
        vector<ByteArray> bytearrays;
        bytearrays.reserve(it->values_size());
        for(auto value : it->values() )
        {
            bytearrays.push_back(ByteArray(value));
        }

        binaryValues.emplace(
            it->header(), 
            ParameterDataRaw::OptionalVector<ByteArray>( 
                bytearrays,
                vector<bool>(it->isnull().begin(), it->isnull().end())
            )
        );
    }

    ParameterDataRaw parameterDataRaw(
        protobufCodec.epoch(), //epoch
        timestamps,
        numericValues,
        stringValues,
        binaryValues,
        tagValues
    );


    return parameterDataRaw;
}
