#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "transport/fw/byteSplitter.h"
#include "transport/fw/byteSplittingModifier.h"
#include "transport/io/IPackage.h"
#include "transport/io/package.h"
#include "transport/fw/exceptions/serializingException.h"

#include <climits>

#include <algorithm>
#include <memory>

using namespace std;
using namespace Quix::Transport;

using ::testing::_;
using ::testing::Lt;
using ::testing::Return;

class MyMockSplitter : public IByteSplitter {
 public:
    MyMockSplitter(int limit) {};
    MOCK_METHOD1( split, std::vector<std::shared_ptr<ByteArrayPackage>>(const std::shared_ptr<ByteArrayPackage>& originalPackage) );
    MOCK_METHOD1( begin, IByteSplitter::Iterator(const std::shared_ptr<ByteArrayPackage>& originalPackage) );
    MOCK_CONST_METHOD1( end  , IByteSplitter::Iterator(const std::shared_ptr<ByteArrayPackage>& originalPackage) );
};

TEST(byteSplittingModifierTest, Modify_ByteSplitDoesntHappen_ShouldRaisePackage) 
{
    // Arrange
    ByteArray bytes = ByteArray::initRandom(500);

    TransportContext transportContext;
    transportContext["Test"] = "123";

    auto package = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    MyMockSplitter splitter(50);

    std::vector<std::shared_ptr<ByteArrayPackage>> ret;
    ret.push_back(package);


    //
    ON_CALL( splitter, begin(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    package,                //originalPackage
                    1,                  //maxMessageSizeWithoutHeader - does not really matter
                    0,                  //messageId does not matter
                    0,                  //index
                    false               //is split message
                )
            )
        );
    ON_CALL( splitter, end(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    package,                //originalPackage
                    1,                  //maxMessageSizeWithoutHeader - does not really matter
                    0,                  //messageId does not matter
                    1,                  //index
                    false               //is split message
                )
            )
        );



    ByteSplittingModifier modifier(&splitter);

    std::vector<std::shared_ptr<ByteArrayPackage>> nonGeneric;
    modifier.onNewPackage += [&](std::shared_ptr<IPackage> package){
        nonGeneric.push_back( dynamic_pointer_cast<ByteArrayPackage>( package ) );
    };


    EXPECT_CALL(splitter, begin(package));
    EXPECT_CALL(splitter, end(package));


    modifier.send(package);

    ASSERT_EQ( nonGeneric.size(), 1 );
    auto& raisedPackage = nonGeneric[0];
    ASSERT_EQ( *raisedPackage, *package );
}

TEST(byteSplittingModifierTest, Modify_ByteSplitDoesntHappen_WithVectorIterator_ShouldRaisePackage) 
{
    // Arrange
    ByteArray bytes = ByteArray::initRandom(500);

    TransportContext transportContext;
    transportContext["Test"] = "123";

    auto package = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    MyMockSplitter splitter(50);

    std::vector<std::shared_ptr<ByteArrayPackage>> ret;
    ret.push_back(package);


    //
    ON_CALL( splitter, begin(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    ret.begin()
                )
            )
        );
    ON_CALL( splitter, end(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    ret.begin(),
                    ret.size()
                )
            )
        );



    ByteSplittingModifier modifier(&splitter);

    std::vector<std::shared_ptr<ByteArrayPackage>> nonGeneric;
    modifier.onNewPackage += [&](std::shared_ptr<IPackage> package){
        nonGeneric.push_back( dynamic_pointer_cast<ByteArrayPackage>( package ) );
    };


    EXPECT_CALL(splitter, begin(package));
    EXPECT_CALL(splitter, end(package));


    modifier.send(package);

    ASSERT_EQ( nonGeneric.size(), 1 );
    auto& raisedPackage = nonGeneric[0];
    ASSERT_EQ( *raisedPackage, *package );
}


TEST(byteSplittingModifierTest, Modify_ByteSplitDoesHappen_ShouldRaisePackagesAndCompletedTask) 
{
    // Arrange
    ByteArray bytes = ByteArray::initEmpty(500);

    TransportContext transportContext;
    transportContext["Test"] = "123";

    auto package = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    MyMockSplitter splitter(50);

    std::vector<std::shared_ptr<ByteArrayPackage>> returned;
    const int splitCount = 5;
    for (int i = 0; i < splitCount; i++)
    {
        ByteArray segmentBytes = ByteArray::initRandom(111);
        returned.push_back(
            std::shared_ptr<ByteArrayPackage>(
                new ByteArrayPackage(segmentBytes, transportContext)
            )
        );
    }


    ON_CALL( splitter, begin(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    returned.begin()
                )
            )
        );
    ON_CALL( splitter, end(_) )
        .WillByDefault(
            Return(
                IByteSplitter::Iterator(
                    returned.begin(),
                    returned.size()
                )
            )
        );



    ByteSplittingModifier modifier(&splitter);

    std::vector<std::shared_ptr<ByteArrayPackage>> nonGeneric;
    modifier.onNewPackage += [&](std::shared_ptr<IPackage> package){
        nonGeneric.push_back( dynamic_pointer_cast<ByteArrayPackage>( package ) );
    };


    EXPECT_CALL(splitter, begin(package));
    EXPECT_CALL(splitter, end(package));


    modifier.send(package);

    ASSERT_EQ( nonGeneric.size(), splitCount );

    for (int index = 0; index < nonGeneric.size() - 1; index++)
    {
        auto& raisedPackage = nonGeneric[index];
        EXPECT_EQ( *raisedPackage, *returned[index] );
    }
}

void Send_SubscribersTakeTimeToCompleteTask_ReturnedTaskShouldOnlyResolveWhenAllPackageTaskResolve(int packageCount){
    // Arrange
    ByteArray bytes = ByteArray::initEmpty(500);

    TransportContext transportContext;
    transportContext["Test"] = "123";

    auto package = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    MyMockSplitter splitter(50);

    std::vector<std::shared_ptr<ByteArrayPackage>> returned;
    for (int i = 0; i < packageCount; i++)
    {
        ByteArray segmentBytes = ByteArray::initRandom(111);
        returned.push_back(
            std::shared_ptr<ByteArrayPackage>(
                new ByteArrayPackage(segmentBytes, transportContext)
            )
        );
    }


    ///TODO: finalize after ByteSplittingModifier.onNewPackage is done async 
}

TEST(byteSplittingModifierTest, Send_SubscribersTakeTimeToCompleteTask_ReturnedTaskShouldOnlyResolveWhenAllPackageTaskResolve_1) 
{
    Send_SubscribersTakeTimeToCompleteTask_ReturnedTaskShouldOnlyResolveWhenAllPackageTaskResolve(1);
}

TEST(byteSplittingModifierTest, Send_SubscribersTakeTimeToCompleteTask_ReturnedTaskShouldOnlyResolveWhenAllPackageTaskResolve_10) 
{
    Send_SubscribersTakeTimeToCompleteTask_ReturnedTaskShouldOnlyResolveWhenAllPackageTaskResolve(10);
}


