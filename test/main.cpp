#include "gtest/gtest.h"

#include <google/protobuf/stubs/common.h>   /* google::protobuf::ShutdownProtobufLibrary */

#include <ctime>       /* time */
#include <cstdlib>     /* srand, rand */


int main(int argc, char **argv)
{
    srand (time(NULL));

    ::testing::InitGoogleTest(&argc, argv);
   ::testing::GTEST_FLAG(filter) = "byteMergingModifierTest*";

    int ret = RUN_ALL_TESTS();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}