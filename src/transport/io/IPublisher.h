#pragma once

#include "./IPackage.h"

#include <memory>


namespace Quix { namespace Transport {

class IPublisher{

public:
    virtual void send(std::shared_ptr<IPackage> package) const = 0;
};

} }