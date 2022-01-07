#pragma once

#include <functional>
#include <memory>

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"

#include "../../utils/eventHandler.h"

#include "./IModifier.h"


namespace Quix { namespace Transport {


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteSplittingModifier: public IModifier{

private:
    IByteSplitter* byteSplitter_;

public:
    /**
     * Initializes a new instance of ByteSplittingModifier
     */
    ByteSplittingModifier(IByteSplitter* byteSplitter);

    /**
     * Send a package, which the modifier splits if necessary. Split results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<IPackage> package);
};

} }