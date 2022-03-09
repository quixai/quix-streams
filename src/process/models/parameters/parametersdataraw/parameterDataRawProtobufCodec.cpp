#include <memory>

#include "./parameterDataRaw.h"
#include "./parameterDataRawProtobufCodec.h"
#include "./parameterDataRawProto.pb.h"

using namespace std;
using namespace Quix::Process;
using namespace Quix::Transport;

template<typename ProtobufT, typename ValueIt, typename DefaultT>
void addElementToProtobufItem(ProtobufT* val, ValueIt it, DefaultT defaultValue)
{
    val->set_header(it->first);

    for( auto& value : it->second )
    {
        if( value.has_value() )
        {
            val->add_isnull(false);
            val->add_values(*value);
        }
        else
        {
            val->add_isnull(true);
            val->add_values(defaultValue);
        }
    }
}

template<typename ElT, typename ValueIt>
std::vector<ElT> loadElementFromProtobufItem(ValueIt it)
{
    vector<ElT> data;
    data.reserve(it->values_size());
    for(int i = 0; i < it->values_size(); ++i)
    {
        if( !it->isnull(i) )
        {
            data[i] = it->values(i);
        }
    }
    return data;
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
    for( map<string, std::vector<std::experimental::optional<string>> >::const_iterator it = parameterDataRaw.stringValues.cbegin() ; it != parameterDataRaw.stringValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_string_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_StringValues, map<string, std::vector<std::experimental::optional<string>>, string >::const_iterator>( val, it, "" );
    }
    // add tags
    for( map<string, std::vector<std::experimental::optional<string>> >::const_iterator it = parameterDataRaw.tagValues.cbegin() ; it != parameterDataRaw.tagValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_tag_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_StringValues, map<string, std::vector<std::experimental::optional<string>>, string >::const_iterator>( val, it, "" );
    }
    // add numeric values
    for( map<string, std::vector<std::experimental::optional<double>> >::const_iterator it = parameterDataRaw.numericValues.cbegin() ; it != parameterDataRaw.numericValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_numeric_values();
        addElementToProtobufItem<::Quix::Process::ParameterDataRawProto_NumericValues, map<string, std::vector<std::experimental::optional<double>>, double >::const_iterator>( val, it, 0. );
    }
    // add binary values
    for( map<string, std::vector<std::experimental::optional<ByteArray>> >::const_iterator it = parameterDataRaw.binaryValues.cbegin() ; it != parameterDataRaw.binaryValues.cend() ; ++it )
    {
        auto val = protobufCodec.add_binary_values();

        val->set_header( it->first );

        for( auto& value : it->second )
        {
            if( value.has_value() )
            {
                val->add_isnull(false);
                val->add_values((*value).data(), (*value).len());
            }
            else
            {
                val->add_isnull(true);
                char text[1] = {0};
                val->add_values(&text,0);
            }
            
            // val->add_values(value.data(), value.len());
        }
        // for( const auto& isnull : it->second.isnull )
        // {
        //     val->add_isnull(isnull);
        // }
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
    map<string, vector<std::experimental::optional<string>> > stringValues;
    for( auto it = protobufCodec.string_values().begin(); it != protobufCodec.string_values().end() ; it++ )
    {
        vector<std::experimental::optional<string>> data;
        data.reserve(it->values_size());
        for(int i = 0; i < it->values_size(); ++i)
        {
            if( !it->isnull(i) )
            {
                data[i] = it->values(i);
            }
        }

        stringValues.emplace(
            it->header(), 
            data
        );
    }

    //load string values
    map<string, vector<std::experimental::optional<string>> > tagValues;
    for( auto it = protobufCodec.tag_values().begin(); it != protobufCodec.tag_values().end() ; it++ )
    {
        vector<std::experimental::optional<string>> data;
        data.reserve(it->values_size());
        for(int i = 0; i < it->values_size(); ++i)
        {
            if( !it->isnull(i) )
            {
                data[i] = it->values(i);
            }
        }

        tagValues.emplace(
            it->header(), 
            data
        );
    }

    //load numeric values
    map<string, vector<std::experimental::optional<double>> > numericValues;
    for( auto it = protobufCodec.numeric_values().begin(); it != protobufCodec.numeric_values().end() ; it++ )
    {
        vector<std::experimental::optional<double>> data;
        data.reserve(it->values_size());
        for(int i = 0; i < it->values_size(); ++i)
        {
            if( !it->isnull(i) )
            {
                data[i] = it->values(i);
            }
        }

        numericValues.emplace(
            it->header(), 
            data
        );
    }

    map<string, vector<std::experimental::optional<ByteArray>> > binaryValues;
    for( auto it = protobufCodec.binary_values().begin(); it != protobufCodec.binary_values().end() ; it++ )
    {

        vector<std::experimental::optional<ByteArray>> data;
        data.reserve(it->values_size());
        for(int i = 0; i < it->values_size(); ++i)
        {
            if( !it->isnull(i) )
            {
                data[i] = ByteArray(it->values(i));
            }
        }


        binaryValues.emplace(
            it->header(), 
            data
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
