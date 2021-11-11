#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/io/package.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "../helpers/passthrough.h"
#include "../helpers/testModel.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

TEST(transportPackageTest, GenericConstructor_ShouldSetEmptyMetadata)
{
    // Arrange
    TestModel model;
    Package<TestModel> package(model);

    // Act
    EXPECT_TRUE( package.metaData().empty() );
}

TEST(transportPackageTest, GenericConstructor_ShouldSetEmptyTransportContext)
{
    // Arrange
    TestModel model;
    Package<TestModel> package(model);

    // Act
    EXPECT_TRUE( package.transportContext().empty() );
}

TEST(transportPackageTest, GenericConstructor_ShouldSetTypeToProvided)
{
    // Arrange
    TestModel model;
    Package<TestModel> package(model);

    // Act
    EXPECT_EQ( package.modelKey() , model.modelKey() );
}

TEST(transportPackageTest, GenericConstructor_ShouldSetValueToProvided)
{
    // Arrange
    TestModel model;
    Package<TestModel> package(model);

    // Act
    EXPECT_EQ( package.value() , model );
}
