#pragma once

#include "./abstractPackage.h"

namespace Quix { namespace Transport {

class AbstractInput{

public:
    virtual void send(AbstractPackage* package) = 0;
};

} }