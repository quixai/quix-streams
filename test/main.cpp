#include "gtest/gtest.h"

#include <ctime>       /* time */
#include <cstdlib>     /* srand, rand */


int main(int argc, char **argv)
{
    srand (time(NULL));

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}