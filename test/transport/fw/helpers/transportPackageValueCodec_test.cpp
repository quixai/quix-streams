#include "gtest/gtest.h"
#include "transport/fw/codecs/codecBundle.h"
#include "transport/fw/helpers/transportPackageValueCodec.h"
#include "transport/fw/helpers/transportPackageValueCodecProtobuf.h"
#include "transport/io/byteArray.h"

#include <algorithm>
#include <memory>
#include <functional>

using namespace std;
using namespace Quix::Transport;

void testDeserialization(size_t size, std::function<ByteArray(std::shared_ptr<TransportPackageValue>)> serializer)
{
    //arrange
    CodecBundle codecBundle(
        ModelKey("modelKey"),
        CodecId("codecId")
    );

    std::shared_ptr<TransportPackageValue> package = 
        std::shared_ptr<TransportPackageValue>(
            new TransportPackageValue(
                std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(
                        ByteArray::initEmpty(size)
                    )
                ),
                codecBundle
            )
        )
    ;

    // Act
    ByteArray raw(serializer(package));
    std::shared_ptr<TransportPackageValue> deserializedPackage = TransportPackageValueCodec::deserialize(raw);

    // Assert
    ASSERT_EQ( *deserializedPackage, *package );
}


///// Test serialize with default codec implementation

TEST(transportPackageValueCodec, checkIncrementingSizes)
{
    for(int i = 0; i < 20*100; i+=100)
    {
        testDeserialization(i, TransportPackageValueCodec::serialize);
    }
}


///// Test serialize with Protobuf codec implementation

TEST(transportPackageValueCodecProtobuf, checkIncrementingSizes)
{
    for(int i = 0; i < 20*100; i+=100){
        testDeserialization(i, TransportPackageValueCodecProtobuf::serialize);
    }
}


