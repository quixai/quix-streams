#pragma once

#include <climits>

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

#include "../../utils/eventHandler.h"

#include "../io/package.h"
#include "../io/IPublisher.h"
#include "./byteSplitter.h"
#include "./byteSplitProtocolHeader.h"

namespace Quix { namespace Transport {



/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class IByteMerger{

public:

    /***
     * Composite key used as lookup into the buffer cache
     */
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

        inline ByteMergerBufferKey(const ByteMergerBufferKey& other) = default;

        inline ByteMergerBufferKey(const std::string& msgGroupKey = "", int msgId = INT_MIN)
         :
         msgId_(msgId),
         msgGroupKey_(msgGroupKey)
        {

        }

        inline bool operator==(const ByteMergerBufferKey& other) const 
        {
            return 
                msgId_ == other.msgId_
                    &&
                msgGroupKey_ == other.msgGroupKey_
                ; 
        }

        inline bool operator<(const ByteMergerBufferKey& other) const 
        {
            if(msgId_ < other.msgId_)
                return true;
            if(msgGroupKey_ < other.msgGroupKey_)
                return true;
            return false;
        }

        inline const std::string& msgGroupKey() const {
            return msgGroupKey_;
        }

    };

    class ByteMergerBufferItem{
    public:
        size_t totalDataLength;
        std::vector<std::shared_ptr<ByteArrayPackage>> receivedPackages;

        inline ByteMergerBufferItem(size_t initialSize = -1)
        :
        totalDataLength(0)
        {
            if(initialSize >= 0)
            {
                //pre allocate array in single step for optimization
                receivedPackages.reserve(initialSize);
            }
        }

        void addPackage(const std::shared_ptr<ByteArrayPackage>& package);
    };

private:

    /**
     * @brief Attempt to merge using string key msgGroupKey
     * 
     * @param originalPackage package to be merged
     * @param msgGroupKey message group key
     * @param outPackage output merged package
     * @return true if the merge returned any package in outPackage
     */
    virtual bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        const std::string&                  msgGroupKey,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    ) = 0;

    /**
     * @brief Attempt to merge using string key msgGroupKey and returning also bufferKey
     * 
     * @param originalPackage package to be merged
     * @param msgGroupKey message group key
     * @param outPackage output merged package
     * @param bufferKey output buffer key
     * @return true if the merge returned any package in outPackage
     */
    virtual bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        const std::string&                  msgGroupKey,
        std::shared_ptr<ByteArrayPackage>&  outPackage,
        ByteMergerBufferKey&                bufferKey
    ) = 0;

public:

    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    EventHandler<const ByteMergerBufferKey&> onMessageSegmentsPurged;


    /**
     * @brief Checks whether the buffer contains at least one message ( package ) with specific buffer key
     * 
     * @param key ByteMergerBufferKey lookup key for the package
     * @return true if the buffer contains key, otherwise false
     */
    virtual bool exists(ByteMergerBufferKey key) = 0;

    /**
     * @brief Checks whether the buffer contains at specific message ( package ) under buffer key
     * 
     * @param key ByteMergerBufferKey lookup key for the package
     * @param msgId messageId
     * @return true if the buffer contains messageId under key, otherwise false
     */
    virtual bool exists(ByteMergerBufferKey key, int msgId) = 0;

    /**
     * @brief Removes all buffered data for the specified buffer key
     * 
     * @param key buffer key
     */
    virtual void purge(const ByteMergerBufferKey& key) = 0;

    /**
     * @brief Attempt to merge using ByteMergerBufferKey
     * 
     * @param originalPackage package to be merged
     * @param bufferId buffer key
     * @param outPackage output merged package
     * @return true if the merge returned any package in outPackage
     */
    bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        ByteMergerBufferKey&                bufferId,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    );

};


/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteMerger : public IByteMerger{

private:

    /// buffer mutex lock
    std::mutex bufferLock_;

    /// all temporary received packages
    std::unordered_map<ByteMergerBufferKey, std::shared_ptr<ByteMergerBufferItem>, ByteMergerBufferKey::Hasher> buffer_;


    bool tryGetHeader(
        const std::shared_ptr<ByteArrayPackage>& originalPackage, 
        ByteSplitProtocolHeader& outHeader
    ) const;

    bool tryAssemblePackage(
        const ByteMergerBufferKey&          key, 
        ByteSplitProtocolHeader&            header,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    );

public:

    ByteMerger();


    /// <summary>
    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    /// </summary>
    bool exists(ByteMergerBufferKey key);

    /// <summary>
    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    /// </summary>
    bool exists(ByteMergerBufferKey key, int msgId);

    /// <summary>
    /// Raised when message segments of the specified buffer id have been purged. Reason could be timout or similar.
    /// </summary>
    void purge(const ByteMergerBufferKey& key);

    bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        const std::string&                  msgGroupKey,
        std::shared_ptr<ByteArrayPackage>&  outPackage
    );

    bool tryMerge(
        std::shared_ptr<ByteArrayPackage>   originalPackage, 
        const std::string&                  msgGroupKey,
        std::shared_ptr<ByteArrayPackage>&  outPackage,
        ByteMergerBufferKey&                bufferKey
    );

};

} }


