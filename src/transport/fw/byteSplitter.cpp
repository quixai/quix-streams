#include "../io/IPublisher.h"
#include "../io/IPackage.h"
#include "./byteSplitter.h"

#include <cstring>
#include <sstream>

#include "./exceptions/serializingException.h"

namespace Quix { namespace Transport {

    ByteSplitter::ByteSplitter(const size_t maxMessageSize) : 
        maxMessageSize_(maxMessageSize), 
        maxMessageSizeWithoutHeader_(maxMessageSize - sizeof(ByteSplitProtocolHeader))
    {
        if( maxMessageSize <= sizeof(ByteSplitProtocolHeader) ) {
            std::stringstream ss;
            ss << "ByteSplitter maxMessageSize must be at least " << sizeof(ByteSplitProtocolHeader);
            // todo: throw better exception type
            throw new SerializingException(ss.str());
        }
    };

    const size_t ByteSplitter::absoluteMaxMessageSize() const {
        return maxMessageSizeWithoutHeader_ * UINT8_MAX;
    } 

    void ByteSplitter::send(std::shared_ptr<RawBytePackage> originalPackage) {
        const auto& originalValue = originalPackage->value(); 
        const size_t originalLen = originalValue.len();
        const uint8_t* originalData = originalValue.data();
        ModelKey modelKey = originalPackage->modelKey();

        if( originalLen > absoluteMaxMessageSize()){
            std::stringstream ss;
            ss << "Message size " << originalLen << " bytes exceeds allowed maximum message size of " << absoluteMaxMessageSize() << " bytes";
            // todo: throw better exception type
            throw new SerializingException(ss.str());
        }

        uint32_t messageId = this->messageId++;
        uint8_t maxIndex = originalLen / maxMessageSizeWithoutHeader_;
        uint8_t curIndex = 0;

        auto& originalMetadata = originalPackage->metaData();

        size_t startDataIndex = 0;       
        do{
            size_t toSendDataLength;
            if( startDataIndex + maxMessageSizeWithoutHeader_ > originalLen ){
                //not the last package
                toSendDataLength = maxMessageSizeWithoutHeader_;
            }else{
                //last package >> leave rest of data
                toSendDataLength = originalLen - startDataIndex;
            }

            //creates the header 
            ByteSplitProtocolHeader packetHeader(messageId, curIndex, maxIndex);

            ByteArray packet = ByteArray::prependHeader(
                (uint8_t*)&packetHeader, //adds pointer into the packetHeader
                sizeof(ByteSplitProtocolHeader),    //header length
                ByteArray(originalValue, startDataIndex, toSendDataLength) //rest of package is subset of original message
            );

            onNewPackage(
                std::shared_ptr<RawBytePackage>(
                    new RawBytePackage(packet, originalMetadata)
                )
            );

            startDataIndex += maxMessageSizeWithoutHeader_;
            curIndex++;
        }while(startDataIndex < originalLen);
    };

} }