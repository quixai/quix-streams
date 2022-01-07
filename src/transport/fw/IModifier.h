#pragma once

#include "../io/transportContext.h"

#include "../../utils/eventHandler.h"

#include "./IRevocation.h"

#include "../io/IPublisher.h"
#include "../io/ISubscriber.h"

#include <memory>
#include <vector>


namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to take Package
 */
class IModifier : public ISubscriber, public IPublisher{
};


} }