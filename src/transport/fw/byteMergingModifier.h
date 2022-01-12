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

#include "./IModifier.h"
#include "./IRevocation.h"

#include "../../utils/eventHandler.h"

namespace Quix { namespace Transport {


/**
 * Modifier, which encapsulates an <see cref="IByteMerger"/> to merge previously split packages
*/
class ByteMergingModifier: public IModifier, public IRevocationSubscriber{
private:

    int bufferOrder_;
    int bufferCounter_;

    /// Packages that are queued up. If they have package value, there is no more fragment to merge
    std::unordered_map< IByteMerger::ByteMergerBufferKey, std::shared_ptr<ByteArrayPackage>, IByteMerger::ByteMergerBufferKey::Hasher > pendingPackages_;
    /// the order the packages should be raised
    std::unordered_map< IByteMerger::ByteMergerBufferKey, long, IByteMerger::ByteMergerBufferKey::Hasher > packageOrder_;

    /// In case of packages that need merging, this package is the one which contains the extra context.
    std::unordered_map< IByteMerger::ByteMergerBufferKey, std::shared_ptr<TransportContext>, IByteMerger::ByteMergerBufferKey::Hasher > firstPackageContext_;

    /// Lock used for every internal structure ( can be in future splitted into multiple mutexes, but for now kept as single for simplicity )
    std::mutex lock_;

    /// Lock used inside raiseNextPackageIfReady to replace SemaphoreSlim
    std::mutex raiseNextPackageIfReadyLock_;
    
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
     * Send a package, which the modifier merged if necessary. Merged results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<IPackage> package);

    void subscribe(IRevocationPublisher* revocationPublisher); 
};

} }


