#include "gtest/gtest.h"
#include "utils/boxedValue.h"

using namespace std;
using namespace Quix;


TEST(boxedValueTest, StrBasic)
{
    // Arrange
    BoxedValue v1("1");
    BoxedValue v1_1("1");
    BoxedValue v2("2");

    // Assert
    EXPECT_EQ( v1, v1_1 );
    EXPECT_NE( v1, v2 );
}

TEST(boxedValueTest, LongBasic)
{
    // Arrange
    BoxedValue v1(1);
    BoxedValue v1_1(1);
    BoxedValue v2(2);

    // Assert
    EXPECT_EQ( v1, v1_1 );
    EXPECT_NE( v1, v2 );
}

TEST(boxedValueTest, Types)
{
    // Arrange
    BoxedValue v1(1);
    BoxedValue s1("1");

    // Assert
    EXPECT_NE( s1, v1 );
}

TEST(boxedValueTest, UndefinedEquals)
{
    EXPECT_EQ( BoxedValue(), BoxedValue() );
}

template<typename T1, typename T2>
void testCompareLT(const T1& v1, const T2& v2)
{
    EXPECT_EQ( v1 < v2, BoxedValue(v1) < BoxedValue(v2) );
};

TEST(boxedValueTest, LongLE)
{
    testCompareLT<int, int>(1, 1);
    testCompareLT<int, int>(1, 2);
    testCompareLT<int, int>(2, 1);
}

TEST(boxedValueTest, StringLE)
{
    testCompareLT<std::string, std::string>("1", "1");
    testCompareLT<std::string, std::string>("1", "2");
    testCompareLT<std::string, std::string>("2", "1");
}


