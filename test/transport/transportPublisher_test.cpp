#include "gtest/gtest.h"
#include "transport/codec/ICodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/io/package.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "./helpers/passthrough.h"
#include "./helpers/testModel.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;


class MyException : public std::exception { };

TEST(transportPublisherTest, ExceptionThrownByInput_ShouldThrowException)
{
    // Arrange
    Passthrough passthrough([](std::shared_ptr<IPackage> val){
        throw MyException();
    });

    TransportPublisher transportPublisher(&passthrough);

    std::shared_ptr<Package<TestModel>> sentPackage(
        new Package<TestModel>(
            TestModel()
        )
    );

    // Act
    EXPECT_THROW({ transportPublisher.send(sentPackage); }, MyException);
}
