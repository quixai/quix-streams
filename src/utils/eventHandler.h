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
 *   holds unique id for the function for being able to map inside the unordered map
 *   https://stackoverflow.com/questions/20692790/define-stdhashstdfunction 
 */

static size_t UNIQID = 1;

template <typename ..._ArgTypes>
class EventHandlerFunction
{
    size_t uid_;
    std::function<void(_ArgTypes...)> callback_;
public:

    EventHandlerFunction( )
     : 
     uid_(0), 
     callback_(nullptr)
    {
        
    }

    EventHandlerFunction( std::function<void(_ArgTypes...)> callback )
     : 
     uid_(++UNIQID), 
     callback_(callback)
    {
    }

    EventHandlerFunction( const EventHandlerFunction<_ArgTypes...>& other )
     : 
     uid_(other.uid_), 
     callback_(other.callback_)
    {

    }

    inline void operator()(_ArgTypes... __args) const
    {
        callback_( __args... );
    }

    size_t uid() const { return uid_; }
};

template <typename ..._ArgTypes>
class EventHandler
{
private:

    /// To work around missing std::function == operator
    size_t getAddress(const EventHandlerFunction<_ArgTypes...>& f) {
        return f.uid();
    };


    /// array where all associated callbacks are stored
    std::vector<EventHandlerFunction<_ArgTypes...>> callbacks_;
    int cnt_ = 0;

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

    inline EventHandler<_ArgTypes...>& operator+=(const EventHandlerFunction<_ArgTypes...>& func)
    {
        // auto functionAddress = getAddress(func);

        // auto it = callbacksSet_.find(functionAddress);
        // if( it != callbacksSet_.end() )
        // {
        //     throw InvalidOperationException("cannot add callback twice");
        // }

        // callbacksSet_.insert(functionAddress);
        callbacks_.push_back(func);
        cnt_++;
        return *this;
    } 

    inline EventHandler<_ArgTypes...>& operator+=(const std::function<void(_ArgTypes...)>& func)
    {
        return *this += EventHandlerFunction<_ArgTypes...>(func);
    }

    inline EventHandler<_ArgTypes...>& operator-=(const EventHandlerFunction<_ArgTypes...>& func)
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
                cnt_--;
                break;
            }
            ++it;
        }
        return *this;
    } 

    inline size_t size() const 
    {
        return callbacks_.size();
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