#pragma once

#include "./IPackage.h"

namespace Quix { namespace Transport {

class ISubscriber{

public:
    std::function<void(std::shared_ptr<IPackage>)> onNewPackage = nullptr;
};

} }