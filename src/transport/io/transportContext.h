#pragma once

#include "../../utils/boxedValue.h"

#include <map>
#include <ostream>
#include <string>

namespace Quix { namespace Transport {

/**
 * Context holder for package when transporting through the pipeline
 */
class TransportContext : public std::map<std::string, Quix::BoxedValue>{

public:
    /**
     * Initializes a new instance of TransportContext that is empty
     */
    inline TransportContext() = default;

    /**
     * Initializes a new instance of TransportContext using the provided map
     * 
     * @param input The map to use as base
     */
    inline TransportContext(
        const std::map<std::string, Quix::BoxedValue>& input
    )
     : 
        std::map<std::string, Quix::BoxedValue>(input) 
    {

    };

    inline bool tryGetValue(const std::string& key, Quix::BoxedValue& out, const Quix::BoxedValue& defaultValue = Quix::BoxedValue()) const
    {
        const auto& it = find(key);
        if( it != end() )
        {
            out = it->second;
            return true;
        }
        else
        {
            out = defaultValue;
            return false;
        }
    };

    inline bool tryGetValue(const std::string& key, std::string& out, const std::string& defaultValue = "") const
    {
        Quix::BoxedValue boxed;
        if( !tryGetValue(key, boxed)  )
        {
            out = defaultValue;
            return false;
        }
        if( !boxed.tryGet(out) )
        {
            out = defaultValue;
            return false;
        }
        return true;
    };

    inline bool tryGetValue(const std::string& key, int64_t& out, const int64_t& defaultValue = -1) const
    {
        Quix::BoxedValue boxed;
        if( !tryGetValue(key, boxed)  )
        {
            out = defaultValue;
            return false;
        }
        if( !boxed.tryGet(out) )
        {
            out = defaultValue;
            return false;
        }
        return true;
    };

    friend std::ostream& operator<<(std::ostream& os, const TransportContext& tc)
    {
        os << ">>>>> TransportContext <<<<<<<" << std::endl;
        for( auto& el : tc )
        {
            os << "[" << el.first.c_str() << "] = " << el.second << std::endl;
        }
        os << ">>>>>>>>>>>>>>  <<<<<<<<<<<<<<" << std::endl;
        return os;
    };

};

class KnownTransportContextKeys
{
    public:
        /// The unique identifier, which groups together messages.
        static const std::string MessageGroupKey;
};


} }