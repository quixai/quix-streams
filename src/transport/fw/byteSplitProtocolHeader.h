#pragma once

#include <functional>
#include <memory>

#include <algorithm>

#include "../io/package.h"
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
    inline ByteSplitProtocolHeader() :
        msgId_(0),
        index_(0),
        maxIndex_(0),
        splitStart_(0),
        splitEnd_(0),
        splitSeparator1_(0),
        splitSeparator2_(0)
    {

    }

    /**
     * Initialize header with parameters
     * 
     * @param msgId messageId
     * @param index index
     * @param maxIndex maxIndex
     */
    inline ByteSplitProtocolHeader(uint32_t msgId, uint8_t index, uint8_t maxIndex) :
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
     * Initialize header from byte array
     */
    inline ByteSplitProtocolHeader(const ByteArray& package)
    {
        (*this) = *((ByteSplitProtocolHeader*)(package.data()));  
    }

    /**
     * Initialize header from byte package
     */
    inline ByteSplitProtocolHeader(const std::shared_ptr<ByteArrayPackage>& package)
    :
    ByteSplitProtocolHeader(package->value())
    {

    }

    /**
     * Check if the protocol header is valid and has all the correct control characters
     */
    inline bool isValid()
    {
        return splitStart_ == ByteSplitStart
            &&
            splitSeparator1_ == ByteSplitSeparator
            &&
            splitSeparator2_ == ByteSplitSeparator
            &&
            splitEnd_ == ByteSplitEnd;
    }

    /**
     * package size in bytes
     */
    inline static const int size()
    {
        return sizeof(ByteSplitProtocolHeader);
    }

    /// get index value of the header
    inline uint8_t index()
    { 
        return index_;
    }

    /// get max index value of the header
    inline uint8_t maxIndex()
    { 
        return maxIndex_;
    }

    /// get message id value of the header
    inline uint32_t msgId()
    {
        return msgId_;
    }
};
#pragma pack()

} }