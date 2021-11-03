#include "../io/abstractInput.h"
#include "../io/abstractPackage.h"
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
            throw new SerializingException(ss.str());
        }
    };

    void ByteSplitter::send(RawBytePackage* originalPackage) {
        const size_t originalLen = originalPackage->value().len();
        const uint8_t* originalData = originalPackage->value().data();
        ModelKey modelKey = originalPackage->modelKey();

        //reuse package if there is no need to splitting
        if( originalLen < maxMessageSizeWithoutHeader_ ){
            onNewPackage(originalPackage);
            return;
        }

        uint32_t messageId = this->messageId++;
        uint8_t maxIndex = originalLen / maxMessageSizeWithoutHeader_;
        uint8_t curIndex = 0;


        try{
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
                uint8_t* newData = new uint8_t[newPacketLen];    //length with header
                
                //hack to headers via using struct
                *((ByteSplitProtocolHeader*)newData) = ByteSplitProtocolHeader(messageId, curIndex, maxIndex);
                memcpy(newData + sizeof(ByteSplitProtocolHeader), originalData + startDataIndex, toSendDataLength);

                onNewPackage(new RawBytePackage(modelKey, RawBytePackageValue(newData, newPacketLen)));

                startDataIndex += maxMessageSizeWithoutHeader_;
                curIndex++;
            }while(startDataIndex < originalLen);


            delete originalPackage;
            delete originalData;
        }catch(...){
            delete originalPackage;
            delete originalData;
            throw;
        }
    };

} }