#pragma once

#include "./IPackage.h"

namespace Quix { namespace Transport {

class IPublisher{

public:
    virtual void send(IPackage* package) = 0;
};

} }