#pragma once

#include <memory>
#include <vector>

#include "../fw/modelKey.h"
#include "./byteArray.h"
#include "./transportContext.h"

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
     * The transport context of the package
     */
    virtual const std::shared_ptr<TransportContext>& transportContext() const = 0;

};

} }