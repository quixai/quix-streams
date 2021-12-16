#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"

#include "./IRevocation.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to take Package
 */
class IModifier{
    //just to make the class polymorphic so we can use the dynamic_cast in the TransportSubscriber
    virtual void dummy() { };
};


} }