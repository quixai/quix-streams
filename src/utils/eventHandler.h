#pragma once

#include <unordered_set>

#include <vector>
#include <functional>
#include <string>

#include <algorithm>

#include "../exceptions/invalidOperationException.h"

namespace Quix { 


/****
 * Wrapper over the std::function<void(_ArgTypes...)>
 *   allows handle event based 
 * 
 * TODO: assigning += same function twice now adds twice to list
 */

template <typename ..._ArgTypes>
class EventHandler
{
private:

    /// To work around missing std::function == operator
    template<typename T, typename... U>
    size_t getAddress(std::function<T(U...)> f) {
        typedef T(fnType)(U...);
        fnType ** fnPointer = f.template target<fnType*>();
        return (size_t) *fnPointer;
    };


    /// array where all associated callbacks are stored
    std::vector<std::function<void(_ArgTypes...)>> callbacks_;

    // /// array where all associated callbacks are stored
    // std::unordered_set<size_t> callbacksSet_;



    // std::function<void(_ArgTypes...)> dispatcher_;

    // template <typename T1 = void>
    // inline std::function<void(T1)> mkDispatcher()
    // {
    //     return std::bind(&EventHandler<T1>::operator(), this, std::placeholders::_1);
    // }

    // template <typename T1, typename T2>
    // inline std::function<void(T1, T2)> mkDispatcher()
    // {
    //     return std::bind(&EventHandler<T1, T2>::operator(), this, std::placeholders::_1, std::placeholders::_2);
    // }


public:

    // inline EventHandler<_ArgTypes...>()
    // {
    //     dispatcher_ = mkDispatcher<_ArgTypes...>();
    // }

    // const std::function<void(_ArgTypes...)>& dispatcher()
    // {
    //     return dispatcher_;
    // }

    inline EventHandler<_ArgTypes...>& operator+=(const std::function<void(_ArgTypes...)>& func)
    {
        // auto functionAddress = getAddress(func);

        // auto it = callbacksSet_.find(functionAddress);
        // if( it != callbacksSet_.end() )
        // {
        //     throw InvalidOperationException("cannot add callback twice");
        // }

        // callbacksSet_.insert(functionAddress);
        callbacks_.push_back(func);
        return *this;
    } 

    inline EventHandler<_ArgTypes...>& operator-=(const std::function<void(_ArgTypes...)>& func)
    {
        // TODO: has O(n) complexity
        // TODO: if function has been assigned twice to list then this functionality does not work

        auto it = callbacks_.cbegin();
        auto end = callbacks_.cend();

        auto functionAddress = getAddress(func);

        // callbacksSet_.erase(functionAddress);

        while( it != end )
        {

            if( getAddress(*it), functionAddress )
            {
                callbacks_.erase(it);
                break;
            }
            ++it;
        }
        return *this;
    } 

    inline void operator()(_ArgTypes... __args) const
    {
        for( auto func: callbacks_ )
        {
            func( __args... );
        }
    }

};

}