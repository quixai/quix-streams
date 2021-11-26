#pragma once

#include <map>
#include <string>

namespace Quix { namespace Transport {

/**
 * Context holder for package when transporting through the pipeline
 */
class TransportContext : public std::map<std::string, std::string>{

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
        const std::map<std::string, std::string>& input
    )
     : 
        std::map<std::string, std::string>(input) 
    {

    };

    inline bool tryGetValue(const std::string& key, std::string& out, const std::string& defaultValue = "") const
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

};

class KnownTransportContextKeys
{
    public:
        /// The unique identifier, which groups together messages.
        static const std::string MessageGroupKey;
};


} }