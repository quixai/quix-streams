#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"
#include "./byteSplitProtocolHeader.h"

namespace Quix { namespace Transport {


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteMerger{

    class ByteMergerBufferKey{
        
    private:
        int msgId_;
        std::string msgGroupKey_;
    public:

        ///Hashing function so the ByteMergerBufferKey can be used inside unordered_map
        class Hasher{
        public:
            inline std::size_t operator()(const ByteMergerBufferKey& k) const
            {
                size_t res = 17;
                res = res * 31 + std::hash<std::string>()( k.msgGroupKey_ );
                res = res * 31 + k.msgId_;
                return res;
            }
        };

        inline ByteMergerBufferKey(const std::string& msgGroupKey, int msgId)
         :
         msgId_(msgId),
         msgGroupKey_(msgGroupKey)
        {

        }

        inline bool operator==(const ByteMergerBufferKey& other) const {
            return 
                msgId_ == other.msgId_
                    &&
                msgGroupKey_ == other.msgGroupKey_
                ; 
        }

        inline bool operator<(const ByteMergerBufferKey& other) const {
            if(msgId_ < other.msgId_)
                return true;
            if(msgGroupKey_ < other.msgGroupKey_)
                return true;
            return false;
        }
    };

    class ByteMergerBufferItem{
    public:
        size_t totalDataLength;
        std::vector<std::shared_ptr<ByteArrayPackage>> receivedPackages;

        inline ByteMergerBufferItem(size_t initialSize = -1)
        {
            if(initialSize >= 0)
            {
                receivedPackages.reserve(initialSize);
            }
        }

        void addPackage(const std::shared_ptr<ByteArrayPackage>& package);
    };


    /// all temporary received packages
    std::unordered_map<ByteMergerBufferKey, std::shared_ptr<ByteMergerBufferItem>, ByteMergerBufferKey::Hasher> buffer_;



    bool tryGetHeader(
        const std::shared_ptr<ByteArrayPackage>& originalPackage, 
        ByteSplitProtocolHeader& outHeader
    ) const;

    bool tryAssemblePackage(
        const ByteMergerBufferKey&          key, 
        std::shared_ptr<ByteArrayPackage>&  outPackage
    );

public:


    /// <summary>
    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    /// </summary>
    std::function<void(ByteMergerBufferKey)> onMessageSegmentsPurged = nullptr;

    /// <summary>
    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    /// </summary>
    void purge(ByteMergerBufferKey key);

    bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        const std::string&                  msgGroupKey,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    );

};

} }


