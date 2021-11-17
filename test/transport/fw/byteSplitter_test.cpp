#include "gtest/gtest.h"
#include "transport/fw/byteSplitter.h"
#include "transport/io/IPackage.h"
#include "transport/fw/exceptions/serializingException.h"

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
    ASSERT_EQ( sizeof(ByteSplitProtocolHeader), 10 );

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

    ByteArray packet = ByteArray::initEmpty(length);

    EXPECT_THROW( 
            splitter.split(
                std::shared_ptr<ByteArrayPackage>(
                    new ByteArrayPackage(packet)
                )
            ) 
            , 
            SerializingException
        );

    // Act
    // Action action = () => splitter.Split(data).ToList();

    // // Assert
    // action.Should().Throw<SerializationException>();

    // EXPECT_ANY_THROW( new ByteSplitter(7) );
}
