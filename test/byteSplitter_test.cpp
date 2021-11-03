#include "gtest/gtest.h"
#include "transport/fw/byteSplitter.h"

#include <algorithm>

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

TEST(byteSplitterTest, doCompilerChecks) {
    // Arrange
    ASSERT_EQ( sizeof(ByteSplitProtocolHeader), 10 );

    ASSERT_TRUE( is_big_endian() );
}

