#pragma once

#include <map>
#include <string>

#include "../../../../transport/io/IModel.h"

namespace Quix { namespace Process {

    class EventDataRaw
    {
    public:

        /**
         *  The timestamp of events in nanoseconds since unix epoch
         */
        long long timestamp;
        
        /**
         *  Tags applied to the event
         */
        std::map<std::string, std::string> tags;
        
        /**
         *  The globally unique identifier of the event
         */
        std::string id;
        
        /**
         *  The value of the event
         */
        std::string value;


    };
} }