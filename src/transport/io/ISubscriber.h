#pragma once

#include "../../utils/eventHandler.h"

#include "../fw/IModifier.h"

#include "./IPackage.h"

namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to push Package to listener
 */
class ISubscriber : public IModifier{

public:
    /**
     * The callback that is used when the ISubscriber has new package for the listener
     */
    EventHandler<std::shared_ptr<IPackage>> onNewPackage;

};

} }