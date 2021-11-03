#pragma once

#include "./abstractPackage.h"

namespace Quix { namespace Transport {

class AbstractOutput{

public:
    virtual void send(AbstractPackage* package) = 0;
};

} }