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
        ByteSplitProtocolHeader header;

        // is this even a split message ?
        if( !tryGetHeader(originalPackage, header) )
        {
            outPackage = originalPackage;
            return true;
        }

        ///TODO: add lock
        //add into buffer
        const ByteMergerBufferKey key( msgGroupKey, header.msgId() );
        if( buffer_.find(key) == buffer_.end() ){
            buffer_.emplace(
                key, 
                shared_ptr<ByteMergerBufferItem>(
                    new ByteMergerBufferItem(header.maxIndex() + 1)
                )
            );
        }
        buffer_[key]->addPackage(originalPackage);
        ///TODO: add lock end

        if( header.index() != header.maxIndex() )
        {
            // we do not have all parts yet if assuming proper message order
            return false;
        }

        //assemble return package
        return tryAssemblePackage(
            key,
            outPackage
        );
    };

    bool ByteMerger::tryGetHeader
    (
        const shared_ptr<ByteArrayPackage>&    originalPackage, 
        ByteSplitProtocolHeader&               outHeader
    ) const
    {
        const auto& originalPackageValue = originalPackage->value();
        if( originalPackageValue.len() < sizeof(ByteSplitProtocolHeader) )
        {
            return false;
        }

        outHeader = *((ByteSplitProtocolHeader*)(originalPackageValue.data()));

        return outHeader.isValid();
    };

    bool ByteMerger::tryAssemblePackage(
        const ByteMergerBufferKey&          key, 
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




        // Merge data into buffer
        ByteArray packet = ByteArray::initEmpty(bufferItem->totalDataLength);

        auto packetSegmentIterator = bufferItem->receivedPackages.cbegin();
        auto packetSegmentIteratorEnd = bufferItem->receivedPackages.cend();

        size_t dataIndex = 0;
        for(;packetSegmentIterator < packetSegmentIteratorEnd; ++packetSegmentIterator){
            const auto& segment = *packetSegmentIterator;

            //TODO: rewrite this operation using only ByteArray for preparation for the ArraySegment
            //pointer to start of the segment data ( after the header )
            auto segmentDataBegin = segment->value().data() + sizeof(ByteSplitProtocolHeader);
            const auto segmentLength = segment->value().len() - sizeof(ByteSplitProtocolHeader);
            //copy data from string to the array
            memcpy(
                packet.data() + dataIndex,     //dst
                segmentDataBegin,                       //src
                segmentLength                           //len
            );
            dataIndex += segmentLength;
        }

        outPackage = std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
            );

        return true;
    }

    void ByteMerger::purge(ByteMerger::ByteMergerBufferKey key)
    {

    }


    void ByteMerger::ByteMergerBufferItem::addPackage(const std::shared_ptr<ByteArrayPackage>& package)
    {
        receivedPackages.push_back(package);
        totalDataLength += package->value().len() - sizeof(ByteSplitProtocolHeader);
    }

} }