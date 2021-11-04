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

    void ByteSplitter::send(RawBytePackage* originalPackage) {
        const size_t originalLen = originalPackage->value().len();
        const uint8_t* originalData = originalPackage->value().data();
        ModelKey modelKey = originalPackage->modelKey();

        try{

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
                size_t newPacketLen = toSendDataLength + sizeof(ByteSplitProtocolHeader);
                const std::shared_ptr<uint8_t> newData(new uint8_t[newPacketLen], std::default_delete<uint8_t[]>());
                
                //hack to headers via using struct
                *((ByteSplitProtocolHeader*)&*newData) = ByteSplitProtocolHeader(messageId, curIndex, maxIndex);
                memcpy(&*newData + sizeof(ByteSplitProtocolHeader), originalData + startDataIndex, toSendDataLength);

                onNewPackage(new RawBytePackage(modelKey, RawBytePackageValue(newData, newPacketLen), originalMetadata));

                startDataIndex += maxMessageSizeWithoutHeader_;
                curIndex++;
            }while(startDataIndex < originalLen);


            delete originalPackage;
        }catch(...){
            delete originalPackage;
            throw;
        }
    };

} }