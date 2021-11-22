#include "../io/IPublisher.h"
#include "../io/IPackage.h"
#include "./byteSplitter.h"

#include <cstring>
#include <sstream>

#include "./exceptions/serializingException.h"

namespace Quix { namespace Transport {


    /****** BEGIN ByteSplitter ************/

    ByteSplitter::ByteSplitter(const size_t maxMessageSize) : 
        maxMessageSize_(maxMessageSize), 
        maxMessageSizeWithoutHeader_(maxMessageSize - ByteSplitProtocolHeader::size())
    {
        if( maxMessageSize <= ByteSplitProtocolHeader::size() ) 
        {
            std::stringstream ss;
            ss << "ByteSplitter maxMessageSize must be at least " << ByteSplitProtocolHeader::size();
            // todo: throw better exception type
            throw SerializingException(ss.str());
        }
    };


    const size_t ByteSplitter::absoluteMaxMessageSize() const 
    {
        return maxMessageSizeWithoutHeader_ * UINT8_MAX;
    } 

    IByteSplitter::Iterator ByteSplitter::begin(const std::shared_ptr<ByteArrayPackage>& originalPackage)
    {
        const size_t originalLen = originalPackage->value().len();

        if(originalLen < maxMessageSize_){
            //not a split message
            return IByteSplitter::Iterator(
                originalPackage,   //originalPackage
                maxMessageSize_,   //maxMessageSizeWithoutHeader 
                0,                 //messageId does not matter
                0,                 //index
                false              //is split message
            );            
        }

        if( originalLen > absoluteMaxMessageSize())
        {
            std::stringstream ss;
            ss << "Message size " << originalLen << " bytes exceeds allowed maximum message size of " << absoluteMaxMessageSize() << " bytes";
            // todo: throw better exception type
            throw SerializingException(ss.str());
        }

        return IByteSplitter::Iterator(
            originalPackage,                //originalPackage
            maxMessageSizeWithoutHeader_,   //maxMessageSizeWithoutHeader 
            this->messageId++ //messageId
        );
    };

    IByteSplitter::Iterator ByteSplitter::end(const std::shared_ptr<ByteArrayPackage>& originalPackage) const
    {
        const auto& originalValue = originalPackage->value(); 
        const size_t originalLen = originalValue.len();

        if(originalLen < maxMessageSize_){
            //not a split message
            return IByteSplitter::Iterator(
                originalPackage,   //originalPackage
                maxMessageSize_,   //maxMessageSizeWithoutHeader 
                0,                 //messageId does not matter
                1,                 //index
                false              //is split message
            );            
        }

        uint8_t maxIndex = originalLen / maxMessageSizeWithoutHeader_;

        //curIndex is maxIndex + 1
        return IByteSplitter::Iterator(
            originalPackage,                //originalPackage
            maxMessageSizeWithoutHeader_,   //maxMessageSizeWithoutHeader 
            0,              //messageId does not matter
            maxIndex+1
        );
    };

    std::vector<std::shared_ptr<ByteArrayPackage>> ByteSplitter::split(const std::shared_ptr<ByteArrayPackage>& originalPackage)
    {
        std::vector<std::shared_ptr<ByteArrayPackage>> ret;
        auto dataIt = begin(originalPackage);
        auto dataEnd = end(originalPackage);
        while( dataIt != dataEnd )
        {
            ret.push_back(*dataIt);
            ++dataIt;
        }
        return ret;
    }


    /****** END ByteSplitter ************/



    /****** BEGIN ByteSplitterIterator ************/
    
    IByteSplitter::Iterator::Iterator(
        const std::shared_ptr<ByteArrayPackage>& originalPackage, 
        size_t maxMessageSizeWithoutHeader, 
        uint32_t messageId, 
        uint8_t curIndex,
        bool splitMessage
    )
    : 
    type_(splitMessage ? IteratorType::splitMessage : IteratorType:: singleMessage),
    curIndex_(curIndex),
    messageId_(messageId),
    originalPackage_(originalPackage),
    maxMessageSizeWithoutHeader_(maxMessageSizeWithoutHeader)
    {

    }

    IByteSplitter::Iterator::Iterator(
        const std::vector<std::shared_ptr<ByteArrayPackage>>::iterator iterator, 
        uint8_t curIndex
    )
    :
    type_(IteratorType::vectorIterator),
    curIndex_(curIndex),
    iterator_(iterator)
    {

    }


    std::shared_ptr<ByteArrayPackage> IByteSplitter::Iterator::operator*() const
    {
        switch(type_){
            case IteratorType::splitMessage:
                goto rest;
            case IteratorType::singleMessage:
                //not split message
                return originalPackage_;
            case IteratorType::vectorIterator:
                return *(iterator_ + curIndex_);
        }

        rest:
        const auto& originalValue = originalPackage_->value(); 
        const size_t originalLen = originalValue.len();

        uint8_t curIndex = curIndex_;
        uint8_t maxIndex = originalLen / maxMessageSizeWithoutHeader_;


        ByteSplitProtocolHeader packetHeader(messageId_, curIndex, maxIndex);

        size_t startDataIndex = curIndex * maxMessageSizeWithoutHeader_;

        if(startDataIndex >= originalLen)
        {
            std::stringstream ss;
            ss << "ByteSplitter Iterator of bound position " << startDataIndex << " ( max allowed " << startDataIndex << ")";
            // todo: throw better exception type
            throw SerializingException(ss.str());
        }

        size_t toSendDataLength;
        if( startDataIndex + maxMessageSizeWithoutHeader_ > originalLen )
        {
            //last package >> leave rest of data
            toSendDataLength = originalLen - startDataIndex;
        }
        else
        {
            //not the last package
            toSendDataLength = maxMessageSizeWithoutHeader_;
        }

        ByteArray packet = ByteArray::prependHeader(
            (uint8_t*)&packetHeader, //adds pointer into the packetHeader
            sizeof(ByteSplitProtocolHeader),    //header length
            ByteArray(originalValue, toSendDataLength, startDataIndex) //rest of package is subset of original message
        );

        return 
            std::shared_ptr<ByteArrayPackage>(
                new ByteArrayPackage(packet)
            );
    }

    /****** END ByteSplitter::Iterator ************/

} }