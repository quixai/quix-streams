#pragma once

#include "./object.h"

namespace Quix { 

template<class T>
class State : public Object,public T
{
public:
    State( ) = default;

    State( const State& other ) = default;

    State( const T& other )
    :
    T( other )
    {

    }
};

}