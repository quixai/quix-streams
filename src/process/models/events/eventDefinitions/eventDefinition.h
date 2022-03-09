#pragma once

#include <vector>

#include "./eventDefinition.h"
#include "../../utility/helpers.h"

#include "../../../../utils/optional.hpp"
#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class EventDefinition
    {
        std::string id_;
        std::string location_;

    public:

        enum EventLevel {
            Trace = 0,
            Debug = 1,
            Information  = 2,
            Warning = 3,
            Error = 4,
            Critical = 5
        };

        inline std::string id() const { return id_; }
        inline void setId( const std::string& i );

        /// The display name of the event
        std::experimental::optional< std::string > name;

        /// Description of the event
        std::experimental::optional< std::string > description;

        /// Optional field for any custom properties that do not exist on the event.
        /// For example this could be a json string, describing all possible event values
        std::experimental::optional< std::string > customProperties;

        /// The level of the event. Defaults to <see cref="EventLevel.Information"/>
        EventLevel level = EventLevel::Information;

        inline std::string location() const { 
            // TODO: add modifying getter
            return location_;
        };
        inline void setLocation( const std::string& l ) { 
            location_ = Helpers::formatLocation(l);
        };

    };
} }