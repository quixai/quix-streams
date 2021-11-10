#pragma once

#include <memory>
#include <vector>

#include "../fw/modelKey.h"
#include "../io/metaData.h"
#include "./byteArray.h"

namespace Quix { namespace Transport {

class IPackage{
public:
    virtual const ModelKey modelKey() const = 0;
    virtual const void* dataptr() const = 0;
    virtual const MetaData metaData() const = 0;
};

} }