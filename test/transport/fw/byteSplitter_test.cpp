#include "gtest/gtest.h"
#include "transport/fw/byteSplitter.h"
#include "transport/io/IPackage.h"
#include "transport/fw/exceptions/serializingException.h"

#include <climits>

#include <algorithm>
#include <memory>

using namespace std;
using namespace Quix::Transport;

int is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } e = { 0x01000000 };

    return e.c[0];
}

TEST(byteSplitterTest, doCompilerChecks) 
{
    // Arrange
    ASSERT_EQ( ByteSplitProtocolHeader::size(), 10 );

    bool is_little_endian = true;
    if( is_big_endian() ) {
        is_little_endian = false;
    }

    //enforce endianness because the ByteSplitProtocolHeader is loading data through cast
    ASSERT_TRUE( is_little_endian );
}

TEST(byteSplitterTest, WithTooLowMaxMessageSize_ShouldThrowArgumentOutOfRangException) 
{
    ByteSplitter* data = nullptr;

    //TODO: implement OutOfRangeException
    EXPECT_ANY_THROW( data = new ByteSplitter(7) );
    if(data != nullptr){
        delete data;
    }
}

TEST(byteSplitterTest, WithDataOutsideAbsoluteMaxSize_ShouldThrowSerializationException) 
{
    // Arrange
    ByteSplitter splitter(50);
    auto length = splitter.absoluteMaxMessageSize() + 1;

    ByteArray packet = ByteArray::initRandom(length);

    // Assert
    EXPECT_THROW( 
            splitter.split(
                std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
                )
            ) 
            , 
            SerializingException
        );
}

TEST(byteSplitterTest, WithDataOutsideAllowedMessageSizeButWithinAbsoluteMaxSize_ShouldReturnSplitBytes) 
{
    // Arrange
    const int maxMsgLength = 50;
    ByteSplitter splitter(maxMsgLength);
    auto length = splitter.absoluteMaxMessageSize() - 10;  // just a bit less than max;

    ByteArray packet = ByteArray::initRandom(length);

    // Act
    auto segments = splitter.split(
                        std::shared_ptr<ByteArrayPackage>(
                                    new ByteArrayPackage(packet)
                        )
                    );
    auto expectedMaxMessageIndex = UINT8_MAX - 1;
    auto expectedMessageId = -1;
    auto dataByteIndex = 0;

    // Assert
    EXPECT_EQ( segments.size(), expectedMaxMessageIndex + 1 );

    for( auto index = 0; index < segments.size(); ++index )
    {
        auto& segment = segments[index];

        ByteSplitProtocolHeader header(segment);

        EXPECT_TRUE( header.isValid() );

        auto messageId = header.msgId();

        if (index == 0)
        {
            ASSERT_NE( messageId, -1 );
            expectedMessageId = messageId;
        }
        else
        {
            ASSERT_EQ( messageId, expectedMessageId );
        }

        ASSERT_EQ( header.index(), index );
        ASSERT_EQ( header.maxIndex(), expectedMaxMessageIndex );
        
        auto messageDataLength = segment->value().len() - ByteSplitProtocolHeader::size();
        for ( auto i = 0; i < messageDataLength; ++i )
        {
            ASSERT_EQ( segment->value().data()[ByteSplitProtocolHeader::size() + i], packet.data()[dataByteIndex] );            
            dataByteIndex++;
        }
    }
    ASSERT_EQ( dataByteIndex, length); // due to last increment, it should actually match the length

}


TEST(byteSplitterTest, WithDataWithAbsoluteMaxSize_ShouldNotThrowSerializationException) 
{
    // Arrange
    ByteSplitter splitter(50);
    auto length = splitter.absoluteMaxMessageSize();

    ByteArray packet = ByteArray::initRandom(length);

    // Assert
    EXPECT_NO_THROW( 
            splitter.split(
                std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
                )
            )
        );
}

TEST(byteSplitterTest, WithDataWithinAllowedMessageSize_ShouldReturnSameBytes) 
{
    // Arrange
    ByteSplitter splitter(50);
    auto length = 48;

    ByteArray packet = ByteArray::initRandom(length);

    // Assert
    const auto segments = splitter.split(
                std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
                )
            );


    EXPECT_EQ( segments.size(), 1 );
    EXPECT_EQ( segments[0]->value(), packet );

}





