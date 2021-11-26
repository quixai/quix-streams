#pragma once

#include <vector>
#include <functional>


namespace Quix { namespace Transport {


/****
 * Wrapper over the std::function<void(_ArgTypes...)>
 *   allows handle event based 
 */

template <typename ..._ArgTypes>
class EventHandler
{
private:
    std::vector<std::function<void(_ArgTypes...)>> callbacks_;
public:
    inline EventHandler<_ArgTypes...>& operator+=(const std::function<void(_ArgTypes...)>& func)
    {
        callbacks_.push_back(func);
        return *this;
    } 

    inline EventHandler<_ArgTypes...>& operator-=(const std::function<void(_ArgTypes...)>& func)
    {
        ///TODO: has O(n) complexity
        const auto& it = callbacks_.find(func);
        if( it != callbacks_.end() )
        {
            callbacks_.erase(it);
        }
        return *this;
    } 

    inline void operator()(_ArgTypes... __args) const{
        for( auto func: callbacks_ )
        {
            func( __args... );
        }
    }

};

} }