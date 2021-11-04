#pragma once

#include "./IPackage.h"

namespace Quix { namespace Transport {

class ISubscriber{

public:
    virtual void send(IPackage* package) = 0;
};

} }