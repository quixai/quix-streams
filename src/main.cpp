#include <iostream>

#include "transport/transport.h"

int main() {
    std::cout << "Hello Easy C++ project!" << std::endl;

    std::cout << "Initializing transport layer" << std::endl;
    Quix::Transport transport(1);

}