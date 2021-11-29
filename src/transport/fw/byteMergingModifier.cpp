#include <string>
#include <memory>
#include <sstream>
#include <algorithm>
#include <functional>

#include "./byteMergingModifier.h"

namespace Quix { namespace Transport {

    using namespace std;

    ByteMergingModifier::ByteMergingModifier(IByteMerger* byteMerger)
    :
    byteMerger_(byteMerger),
    bufferOrder_(0),
    bufferCounter_(0)
    {
        byteMerger->onMessageSegmentsPurged += std::bind( &ByteMergingModifier::onMessageSegmentsPurgedInternal, this, std::placeholders::_1 );
    }

    void ByteMergingModifier::onMessageSegmentsPurgedInternal( const IByteMerger::ByteMergerBufferKey& bufferKey )
    {
        if ( this->removeFromBuffer(bufferKey) )
        {
            this->raiseNextPackageIfReady();
        }
    }


    void ByteMergingModifier::send(std::shared_ptr<IPackage> package)
    {
        /// TODO: add cancellation token

        
        const auto originalPackage = dynamic_pointer_cast<ByteArrayPackage>(package);
        shared_ptr<ByteArrayPackage>               outPackage;
        IByteMerger::ByteMergerBufferKey           bufferKey;

        if( 
            !byteMerger_->tryMerge(
                originalPackage,
                bufferKey,
                outPackage
            )
        )
        {
            if( bufferKey == IByteMerger::ByteMergerBufferKey() )
            {
                return;
            }

            tryAddToBuffer( bufferKey, std::shared_ptr<ByteArrayPackage>(nullptr), originalPackage->transportContext() );

            return;
        }


        // By this point the merged package bytes can't be null, meaning that it was either a package that never had
        // any merging to do, or it was a package segment which completed a full package.
        std::shared_ptr<ByteArrayPackage> packageToRaise(nullptr);
        if( bufferKey == IByteMerger::ByteMergerBufferKey() )
        {
            // null buffer id means that this is not a merged package
            // lets use original package completely
            packageToRaise = originalPackage;
        }
        else
        {
            // buffer id means that this is a merged package

            // TODO: handle transport context
            packageToRaise = outPackage;
        }
        
        // check if empty. We're not worried about threading here, because this method is designed to be invoked via single thread
        // and any external thread will only ever reduce it, not increment. (see OnMessageSegmentsPurged)

        int cnt;
        {
            std::lock_guard<std::mutex> guard(lock_);
            cnt = bufferCounter_;
        }

        if (cnt == 0)
        {
            removeFromBuffer(bufferKey);

            onNewPackage(packageToRaise);
            return;
        }
        
        // Not empty, check if this is next in line
        if ( bufferKey == IByteMerger::ByteMergerBufferKey() )
        {
            // can't be next in line. No buffer id tells us it isn't a buffered value. Given there are other values in the buffer already, this can't possibly be the next.
            tryAddToBuffer(bufferKey, originalPackage, originalPackage->transportContext() );
        }
        else
        {

            // Could be next, but we don't know yet. Let's update in the buffer
            const auto& pendingPackageIterator = pendingPackages_.find(bufferKey);
            if( pendingPackageIterator != pendingPackages_.end() )
            {
                if( pendingPackageIterator->second.get() == nullptr )
                {
                    pendingPackageIterator->second = packageToRaise;
                }
            }

        }

        raiseNextPackageIfReady();
    };

    void ByteMergingModifier::raiseNextPackageIfReady( )
    {
        //TODO: add concurrency ( semaphore )

        vector<pair<IByteMerger::ByteMergerBufferKey, long>> sortedPackageOrder;

        std::copy(
                    packageOrder_.begin(), packageOrder_.end(),
                    back_inserter(sortedPackageOrder)
                   );
        sort(
            sortedPackageOrder.begin(), 
            sortedPackageOrder.end(), 
            [](const pair<IByteMerger::ByteMergerBufferKey, long> & a, const pair<IByteMerger::ByteMergerBufferKey, long> & b) -> bool
            { 
                return b.second > a.second; 
            }
        );

        for( const auto& el : sortedPackageOrder )
        {
            const auto& nextBufferId = el.first;

            std::shared_ptr<ByteArrayPackage> nextPackage(nullptr);
            {
                std::lock_guard<std::mutex> guard(lock_);
                
                const auto& pendingPackageIterator = pendingPackages_.find(nextBufferId);
                if( pendingPackageIterator == pendingPackages_.end() )
                {
                    continue;
                }
                nextPackage = pendingPackageIterator->second;
            }

            if( nextPackage.get() == nullptr )
            {
                // the next package in line is not yet ready.
                return;
            }
            onNewPackage(nextPackage); 
            removeFromBuffer(nextBufferId);
        }

    };

    bool ByteMergingModifier::tryAddToBuffer(
        IByteMerger::ByteMergerBufferKey& bufferId, 
        std::shared_ptr<ByteArrayPackage> value, 
        const std::shared_ptr<TransportContext>& transportContext 
    )
    {

        std::lock_guard<std::mutex> guard(lock_);

        long order; 
        if (bufferId == IByteMerger::ByteMergerBufferKey() )    //is default (empty) value
        {
            order = ++bufferOrder_;

            std::stringstream ss;
            ss << order << ByteSplitProtocolHeader::size();

            bufferId = ss.str(); // not worried about buffer removal here, because only case this should happen if it never was buffered

            if( pendingPackages_.find(bufferId) == pendingPackages_.end() )
            {
                pendingPackages_[bufferId] = value;
            }else
            {
                return false;
            }
        }
        else
        {
            if( pendingPackages_.find(bufferId) == pendingPackages_.end() )
            {
                pendingPackages_[bufferId] = value;
            }
            else
            {
                return false;
            }

            order = ++bufferOrder_;
        }

        //equivalent to pendingPackages.TryAdd(bufferId)
        if( packageOrder_.find(bufferId) == packageOrder_.end() )
        {
            packageOrder_[bufferId] = order;
        }


        ++bufferCounter_;
        return true;
    }



    bool ByteMergingModifier::removeFromBuffer(
        const IByteMerger::ByteMergerBufferKey& bufferId
    )
    {

        {
            std::lock_guard<std::mutex> guard(lock_);

            const auto& pendingPackageIterator = pendingPackages_.find(bufferId);
            if( pendingPackageIterator != pendingPackages_.end() )
            {
                pendingPackages_.erase(pendingPackageIterator);
            }

        
            const auto& packageOrderIterator = packageOrder_.find(bufferId);
            if( packageOrderIterator != packageOrder_.end() )
            {
                packageOrder_.erase(packageOrderIterator);
            }
            else
            {
                // this is not a split package. It is a queued package that is already whole and and isn't buffer
                return false;
            }
        }

        byteMerger_->purge(bufferId);

        {
            std::lock_guard<std::mutex> guard(lock_);
            auto bOrder = bufferOrder_;
            if(--bufferOrder_ == 0){
                // if still the value we thought before resetting buffer counter to 0, then set to 0 also. Overflow would not likely be a problem ever, but better safe
                if(bufferCounter_ == bOrder){
                    bufferCounter_ = 0;
                }
            }
        }

        return true;
    }



} }