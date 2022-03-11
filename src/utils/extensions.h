#pragma once

#include <vector>
#include <list>

namespace Quix { 

template<typename T>
std::vector<T> listToVec(std::list<T> list)
{
    std::vector<T> toCommit;

    int size = list.size();

    toCommit.reserve( size );
    auto it = list.begin( );
    for( int i = 0; i < size ; ++i, ++it )
    {
        toCommit.push_back( *it );
    }

    return toCommit;
};

}
