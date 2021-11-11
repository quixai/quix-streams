#pragma once

#include <functional>
#include <memory>

#include "../io/Package.h"
#include "../io/IPublisher.h"

namespace Quix { namespace Transport {

static const uint8_t ByteSplitStart = '<';
static const uint8_t ByteSplitEnd = '>';
static const uint8_t ByteSplitSeparator = '/';

/**
 * Definition of the ByteSplitter header
 * 
 * Used to simplify definition of the protocol
*/
#pragma pack(push,1)
class ByteSplitProtocolHeader{
private:
    /**
     *  !!!!!!IMPORTANT!!!!!
     * 
     *  The order and size of these variables define the header
     */

    ///Start control character
    uint8_t splitStart_;
    /// message id
    uint32_t msgId_;
    ///Separate control character
    uint8_t splitSeparator1_;
    /// index
    uint8_t index_;
    ///Separate control character
    uint8_t splitSeparator2_;
    /// maximum index
    uint8_t maxIndex_;
    ///End control character
    uint8_t splitEnd_;

public:
    /**
     * Initialize header with unknown values
     * 
     * Note: values can be random
     */
    ByteSplitProtocolHeader()
    {

    }

    /**
     * Initialize header with parameters
     * 
     * @param msgId messageId
     * @param index index
     * @param maxIndex maxIndex
     */
    ByteSplitProtocolHeader(uint32_t msgId, uint8_t index, uint8_t maxIndex) :
        msgId_(msgId),
        index_(index),
        maxIndex_(maxIndex),
        splitStart_(ByteSplitStart),
        splitEnd_(ByteSplitEnd),
        splitSeparator1_(ByteSplitSeparator),
        splitSeparator2_(ByteSplitSeparator)
    {
        
    }
    
    /**
     * Check if the protocol header is valid and has all the correct control characters
     */
    bool isValid()
    {
        return splitStart_ == ByteSplitStart
            &&
            splitSeparator1_ == ByteSplitSeparator
            &&
            splitSeparator2_ == ByteSplitSeparator
            &&
            splitEnd_ == ByteSplitEnd;
    }

    /// get index value of the header
    uint8_t index()
    { 
        return index_;
    }

    /// get max index value of the header
    uint8_t maxIndex()
    { 
        return maxIndex_;
    }

    /// get message id value of the header
    uint32_t msgId()
    {
        return msgId_;
    }
};
#pragma pack()

/**
 * Component for splitting a single array of bytes into multiple according to implementation
*/
class ByteSplitter{

private:
    uint32_t messageId;
    ///max length of message including header
    size_t maxMessageSize_;
    ///max length of message without header
    size_t maxMessageSizeWithoutHeader_;

public:
    /**
     * Initializes a new instance of ByteSplitter
     * 
     * @param maxMessageSize The maximum message size
     */
    ByteSplitter(const size_t maxMessageSize);

    /**
     * 
     * The callback that is used when the split package is available
     */
    std::function<void(std::shared_ptr<RawBytePackage>)> onNewPackage;

    /**
     * Get maximum message size of packet
     */
    const size_t absoluteMaxMessageSize() const;

    /**
     * Send a package, which the modifier splits if necessary. Split results are raised via onNewPackage
     * 
     * @param package The package to split
     */
    void send(std::shared_ptr<RawBytePackage> package);
};

} }