// A2DD.cpp
#include "transport.h"

#include <iostream>


namespace Quix::Transport {

    Transport::Transport(int x)
    {
        gx = x;
    }


    int Transport::get()
    {
        return gx;
    }

}