#pragma once

#include <functional>

#include "../io/IPublisher.h"

namespace Quix { namespace Transport {

static const uint8_t ByteSplitStart = '<';
static const uint8_t ByteSplitEnd = '>';
static const uint8_t ByteSplitSeparator = '/';

#pragma pack(push,1)
class ByteSplitProtocolHeader{
private:
    uint8_t splitStart_;
    uint32_t msgId_;
    uint8_t splitSeparator1_;
    uint8_t index_;
    uint8_t splitSeparator2_;
    uint8_t maxIndex_;
    uint8_t splitEnd_;
public:
    ByteSplitProtocolHeader(){}
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
    bool isValid(){
        return splitStart_ == ByteSplitStart
            &&
            splitSeparator1_ == ByteSplitSeparator
            &&
            splitSeparator2_ == ByteSplitSeparator
            &&
            splitEnd_ == ByteSplitEnd;
    }
    uint8_t index(){ return index_; }
    uint8_t maxIndex(){ return maxIndex_; }
    uint32_t msgId(){ return msgId_; }
};
#pragma pack()

class ByteSplitter{
private:
    uint32_t messageId;
    //max length of message including header
    size_t maxMessageSize_;
    //max length of message without header
    size_t maxMessageSizeWithoutHeader_;
public:
    ByteSplitter(const size_t maxMessageSize);
    std::function<void(RawBytePackage*)> onNewPackage;
    const size_t absoluteMaxMessageSize() const;

    void send(RawBytePackage* package);
};

} }