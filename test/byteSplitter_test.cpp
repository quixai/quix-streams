#include "gtest/gtest.h"
#include "transport/fw/byteSplitter.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

TEST(byteSplitterTest, doCompilerChecks) {
    // Arrange
    ASSERT_EQ( sizeof(ByteSplitProtocolHeader), 10 );

    //TODO: assert endianness
}

