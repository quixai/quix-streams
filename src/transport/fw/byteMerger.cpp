#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include "./byteMerger.h"

namespace Quix { namespace Transport {

    using namespace std;

    bool ByteMerger::tryMerge
    (
        shared_ptr<ByteArrayPackage>    originalPackage, 
        const std::string&              msgGroupKey,
        shared_ptr<ByteArrayPackage>&   outPackage
        
    )
    {
        IByteMerger::ByteMergerBufferKey bufferKey;
        return tryMerge(
            originalPackage, 
            msgGroupKey,
            outPackage,
            bufferKey
        );
    }

    bool ByteMerger::tryMerge
    (
        shared_ptr<ByteArrayPackage>                originalPackage, 
        const std::string&                          msgGroupKey,
        shared_ptr<ByteArrayPackage>&               outPackage,
        IByteMerger::ByteMergerBufferKey&           bufferKey
    )
    {
        ByteSplitProtocolHeader header;

        // is this even a split message ?
        if( !tryGetHeader(originalPackage, header) )
        {
            outPackage = originalPackage;
            return true;
        }

        ///TODO: add lock
        //add into buffer
        bufferKey = IByteMerger::ByteMergerBufferKey( msgGroupKey, header.msgId() );
        if( buffer_.find(bufferKey) == buffer_.end() ){
            buffer_.emplace(
                bufferKey, 
                shared_ptr<ByteMergerBufferItem>(
                    new ByteMergerBufferItem(header.maxIndex() + 1)
                )
            );
        }
        buffer_[bufferKey]->addPackage(originalPackage);
        ///TODO: add lock end

        if( header.index() != header.maxIndex() )
        {
            // we do not have all parts yet if assuming proper message order
            return false;
        }

        //assemble return package
        return tryAssemblePackage(
            bufferKey,
            header,
            outPackage
        );
    };

    bool IByteMerger::tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        ByteMergerBufferKey&                bufferId,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    )
    {
        auto& packageBytes = originalPackage->value();
        std::string key;
        originalPackage->transportContext().tryGetValue(KnownTransportContextKeys::MessageGroupKey, key);
        return this->tryMerge(originalPackage, key, outPackage, bufferId);
    }


    bool ByteMerger::tryGetHeader
    (
        const shared_ptr<ByteArrayPackage>&    originalPackage, 
        ByteSplitProtocolHeader&               outHeader
    ) const
    {
        const auto& originalPackageValue = originalPackage->value();
        if( originalPackageValue.len() < ByteSplitProtocolHeader::size() )
        {
            return false;
        }

        outHeader = ByteSplitProtocolHeader(originalPackageValue);

        return outHeader.isValid();
    };

    bool ByteMerger::tryAssemblePackage(
        const IByteMerger::ByteMergerBufferKey&          key, 
        ByteSplitProtocolHeader&            header,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    ) {

        ///TODO: add lock

        const auto it = buffer_.find(key);
        if( it == buffer_.end() ) {
            // message never was or no longer in buffer !
//          logger.LogDebug("Received last segment for {0}, but rest of the segments can no longer be found.", msgGroupId);
            return false;
        }
        const auto bufferItem = it->second;
        buffer_.erase(it);

        ///TODO: add lock END




        auto packetSegmentIterator = bufferItem->receivedPackages.cbegin();
        auto packetSegmentIteratorEnd = bufferItem->receivedPackages.cend();

        // Empty message or not all packets received
        const auto packetsNo = packetSegmentIteratorEnd - packetSegmentIterator;
        if( packetsNo == 0 || packetsNo != header.maxIndex() + 1 )
        {
            return false;
        }

        // Merge data into buffer
        ByteArray packet = ByteArray::initEmpty(bufferItem->totalDataLength);

        size_t dataIndex = 0;
        for(;packetSegmentIterator < packetSegmentIteratorEnd; ++packetSegmentIterator){
            const auto& segment = *packetSegmentIterator;

            //TODO: rewrite this operation using only ByteArray for preparation for the ArraySegment
            //pointer to start of the segment data ( after the header )
            auto segmentDataBegin = segment->value().data() + ByteSplitProtocolHeader::size();
            const auto segmentLength = segment->value().len() - ByteSplitProtocolHeader::size();
            //copy data from string to the array
            memcpy(
                packet.data() + dataIndex,     //dst
                segmentDataBegin,              //src
                segmentLength                  //len
            );
            dataIndex += segmentLength;
        }

        outPackage = std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
            );

        return true;
    }

    void ByteMerger::purge(const IByteMerger::ByteMergerBufferKey& key)//const std::string& msgGroupKey)
    {
        ///TODO: add lock
        const auto it = buffer_.find(key);
        if( it != buffer_.end() )
        {
            buffer_.erase(it);
        }
        ///TODO: add lock END


        // ///TODO: add lock

        // //find all keys to remove
        // vector<ByteMergerBufferKey> keysToRemove;
        // const auto end = buffer_.end();
        // for( auto it = buffer_.begin(); it != end ; ++it ){
        //     if( it->first.msgGroupKey() == msgGroupKey )
        //     {
        //         keysToRemove.push_back(it->first);
        //     }
        // }

        // //remove those keys
        // for( auto it = keysToRemove.begin(); it != keysToRemove.end(); ++it )
        // {
        //     buffer_.erase(*it);
        // }

        // ///TODO: add lock END
    }

    bool ByteMerger::exists(IByteMerger::ByteMergerBufferKey key)
    {
        ///TODO: add lock
        return buffer_.find(key) != buffer_.end();
        ///TODO: add lock END
    }

    bool ByteMerger::exists(IByteMerger::ByteMergerBufferKey key, int msgId)
    {
        ///TODO: add lock
        const auto it = buffer_.find(key);
        if( it == buffer_.end() ) {
            return false;
        }
        const auto& receivedPackages = it->second->receivedPackages;
        const auto findIt = std::find_if(
            receivedPackages.begin(), 
            receivedPackages.end(), 
            [&](const std::shared_ptr<ByteArrayPackage>& val){
                return ByteSplitProtocolHeader(val).msgId() == msgId;
            }
        );
        return findIt != receivedPackages.end();
        ///TODO: add lock END
    }



    void ByteMerger::ByteMergerBufferItem::addPackage(const std::shared_ptr<ByteArrayPackage>& package)
    {
        receivedPackages.push_back(package);
        totalDataLength += package->value().len() - ByteSplitProtocolHeader::size();
    }

} }