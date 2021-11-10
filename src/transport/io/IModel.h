#pragma once

#include "../fw/modelKey.h"
#include <memory>

namespace Quix { namespace Transport {

class IModel{
public:
    virtual const ModelKey modelKey() const = 0;
};

} }