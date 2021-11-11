#pragma once

#include <memory>
#include <vector>

#include "../fw/modelKey.h"
#include "../io/metaData.h"
#include "./byteArray.h"

namespace Quix { namespace Transport {

/**
 * Interface that defines the behavior of Package.
 * 
 * Package holds a value and its metadata with extra context relevant for transporting it.
 */
class IPackage{

public:
    /**
     * Get ModelKey of the underlying wrapped value
     */
    virtual const ModelKey modelKey() const = 0;

    /**
     * The content meta data
     */
    virtual const MetaData metaData() const = 0;

};

} }