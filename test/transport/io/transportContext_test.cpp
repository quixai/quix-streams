#include "gtest/gtest.h"
#include "transport/io/transportContext.h"

#include <algorithm>
#include <map>
#include <string>

using namespace std;
using namespace Quix::Transport;

TEST(transportContextTest, GenericConstructor_DefaultEmpty)
{
    // Arrange
    TransportContext context;

    // Act
    EXPECT_TRUE( context.empty() );
}

TEST(transportContextTest, GenericConstructor_FromDict)
{
    // Arrange
    map<string, Quix::BoxedValue> map;
    map["key"] = "value";
    TransportContext context(map);

    // Act
    EXPECT_EQ( context["key"], string("value") );
}

