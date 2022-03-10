#pragma once

#include <string>
#include <vector>
#include <map>
#include <optional>

#include "../../../../utils/optional.hpp"

#include "../../../../transport/io/IModel.h"
#include "../../../../transport/io/byteArray.h"
#include "../../../../transport/io/transportContext.h"

namespace Quix { namespace Process {


    class ParameterDataRaw : public Quix::Transport::IModel
    {

    public:

        template<typename T>
        struct OptionalVector
        {
            OptionalVector() {};
            std::vector<std::experimental::optional<T>> values;
        };


        ParameterDataRaw( ) 
        {
            
        };

        ParameterDataRaw(
            long long epoch, 
            std::vector<long long> timestamps,
            std::map< std::string, std::vector<std::experimental::optional<double>> > numericValues,
            std::map< std::string, std::vector<std::experimental::optional<std::string>> > stringValues,
            std::map< std::string, std::vector<std::experimental::optional<Quix::Transport::ByteArray>> > binaryValues,
            std::map< std::string, std::vector<std::experimental::optional<std::string>> > tagValues
        )
         : 
        epoch(epoch),
        timestamps(timestamps),
        numericValues(numericValues),
        stringValues(stringValues),
        binaryValues(binaryValues),
        tagValues(tagValues)
        {

        };

        /**
         * @brief The unix epoch from, which all other timestamps in this model are measured from in nanoseconds.
         * 0 = UNIX epoch (01/01/1970)
         */
        long long epoch;

        /**
         * @brief The timestamps of values in nanoseconds since <see cref="Epoch"/>.
         * Timestamps are matched by index to <see cref="NumericValues"/>, <see cref="StringValues"/>, <see cref="BinaryValues"/>and <see cref="TagValues"/>
         */
        std::vector<long long> timestamps;

        /**
         *  The numeric values for parameters.
         *  The key is the parameter Id the values belong to
         *  The value is the numerical values of the parameter. Values are matched by index to <see cref="Timestamps"/>
         */
        std::map< std::string, std::vector<std::experimental::optional<double>> > numericValues;

        /**
         *  The string values for parameters.
         *  The key is the parameter Id the values belong to
         *  The value is the string values of the parameter. Values are matched by index to <see cref="Timestamps"/>
         */
        std::map< std::string, std::vector<std::experimental::optional<std::string>> > stringValues;

        /**
         *  The tag values for parameters.
         *  The key is the parameter Id the values belong to
         *  The value is the tag values of the parameter. Values are matched by index to <see cref="Timestamps"/>
         */
        std::map< std::string, std::vector<std::experimental::optional<std::string>> > tagValues;


        /**
         *  The binary values for parameters.
         *  The key is the parameter Id the values belong to
         *  The value is the binary values of the parameter. Values are matched by index to <see cref="Timestamps"/>
         */
        std::map<std::string, std::vector<std::experimental::optional<Quix::Transport::ByteArray>> > binaryValues;

        std::shared_ptr<Quix::Transport::TransportContext> transportContext;


        /**
         * Get Model Key definition of the ByteArray model
         *  this is implemented to satisty IModel interface
         *  so the ByteArray can be loaded inside Package
         */
        inline const Quix::Transport::ModelKey modelKey() const 
        { 
            return Quix::Transport::ModelKey::forType<ParameterDataRaw>();
        };
    };
} }