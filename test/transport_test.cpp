#include "gtest/gtest.h"
#include "transport/transport.h"

TEST(blaTest, test1) {
    //arrange
    Quix::Transport transport(5);
    //act
    //assert
    EXPECT_EQ (transport.get (),  5);
}