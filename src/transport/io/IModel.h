#pragma once

#include "../fw/modelKey.h"
#include <memory>

namespace Quix { namespace Transport {

/**
 * Interface that defines the behavior of any model
 */
class IModel{

public:
    /**
     * Get ModelKey representation of the Model
     */
    virtual const ModelKey modelKey() const = 0;
};

} }