#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"
#include "./byteMerger.h"

namespace Quix { namespace Transport {


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteMergingModifier: public IPublisher{
private:

    ByteMerger byteMerger_;

public:
    /**
     * 
     * The callback that is used when the split package is available
     */
    std::function<void(std::shared_ptr<ByteArrayPackage>)> onNewPackage;

    /**
     * Send a package, which the modifier splits if necessary. Split results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<IPackage> package) const;
};

} }


