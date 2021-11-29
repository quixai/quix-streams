#pragma once

#include <functional>
#include <memory>

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"

#include "../../utils/eventHandler.h"

namespace Quix { namespace Transport {


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteSplittingModifier: public IPublisher{

private:
    IByteSplitter* byteSplitter_;

public:
    /**
     * Initializes a new instance of ByteSplittingModifier
     */
    ByteSplittingModifier(IByteSplitter* byteSplitter);

    /**
     * The callback that is used when the split package is available
     */
    EventHandler<std::shared_ptr<ByteArrayPackage>> onNewPackage;

    /**
     * Send a package, which the modifier splits if necessary. Split results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<IPackage> package);
};

} }