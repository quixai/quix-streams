#pragma once

#include <vector>

#include "../../utility/helpers.h"

#include "../../../../exceptions/argumentOutOfRangeException.h"
#include "../../../../utils/optional.hpp"
#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class ParameterDefinition
    {
        std::string id_;
        std::string location_;

    public:

        inline std::string id() const { return id_; }
        void setId( const std::string& i );

        /// The display name of the event
        std::experimental::optional< std::string > name;

        /// Description of the event
        std::experimental::optional< std::string > description;

        /// Minimum value of the parameter
        std::experimental::optional< double > minimumValue;

        /// Maximum value of the parameter
        std::experimental::optional< double > maximumValue;

        /// Unit of the parameter
        std::experimental::optional< std::string > unit;

        /// The formatting to apply on the value for display purposes
        std::experimental::optional< std::string > format;

        /// Optional field for any custom properties that do not exist on the event.
        /// For example this could be a json string, describing all possible event values
        std::experimental::optional< std::string > customProperties;


        inline std::string location() const { 
            // TODO: add modifying getter
            return location_;
        };
        inline void setLocation( const std::string& l ) { 
            location_ = Helpers::formatLocation(l);
        };

    };
} }