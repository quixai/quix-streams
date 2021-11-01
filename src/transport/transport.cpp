// A2DD.cpp
#include "transport.h"

#include <iostream>


namespace Quix {

    Transport::Transport(int x)
    {
        gx = x;

        std::cout << "Transport layer here" << std::endl;
    }


    int Transport::get()
    {
        return gx;
    }

}