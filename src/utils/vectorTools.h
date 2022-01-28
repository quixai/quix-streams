#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>

namespace Quix { 

template<class Inp, class K, class V>
class VectorTools{

public:
    inline static std::map<K, std::vector<V>> groupBy( const std::vector<Inp>& vec, std::function<K(const Inp&)> getKey, std::function<V(const Inp&)> getValue )
    {
        std::map<K, std::vector<V>> ret;
        for( auto& el : vec )
        {
            auto key = getKey(el);
            if( ret.find(key) == ret.end() )
            {
                ret[key] = std::vector<V>();
            }
            ret[key].push_back( getValue(el) );
        }
        return ret;
    }
};

}