#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <atomic>

#include <mutex>


#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"
#include "./byteMerger.h"

#include "../../utils/eventHandler.h"

namespace Quix { namespace Transport {


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteMergingModifier: public IPublisher{
private:

    int bufferOrder_;
    int bufferCounter_;

    /// Packages that are queued up. If they have package value, there is no more fragment to merge
    std::unordered_map< IByteMerger::ByteMergerBufferKey, std::shared_ptr<ByteArrayPackage>, IByteMerger::ByteMergerBufferKey::Hasher > pendingPackages_;
    /// the order the packages should be raised
    std::unordered_map< IByteMerger::ByteMergerBufferKey, long, IByteMerger::ByteMergerBufferKey::Hasher > packageOrder_;

    std::mutex lock_;
    
    IByteMerger* byteMerger_;




    void raiseNextPackageIfReady();

    void onMessageSegmentsPurgedInternal(const IByteMerger::ByteMergerBufferKey& bufferId);

    bool tryAddToBuffer(
        IByteMerger::ByteMergerBufferKey& bufferId, 
        std::shared_ptr<ByteArrayPackage> package, 
        const std::shared_ptr<TransportContext>& transportContext 
    );

    bool removeFromBuffer(
        const IByteMerger::ByteMergerBufferKey& bufferId
    );

public:

    ByteMergingModifier(IByteMerger* byteMerger_);

    /**
     * The callback that is used when the split package is available
     */
    EventHandler<std::shared_ptr<ByteArrayPackage>> onNewPackage;

    /**
     * Send a package, which the modifier merged if necessary. Merged results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<IPackage> package);
};

} }


