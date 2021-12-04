#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "transport/fw/byteMerger.h"
#include "transport/fw/byteMergingModifier.h"
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
using ::testing::Eq;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::SetArgReferee;

class MyMockMerger : public IByteMerger {

 public:

    MOCK_METHOD1( exists ,    bool(ByteMergerBufferKey key)               );
    MOCK_METHOD2( exists ,    bool(ByteMergerBufferKey key, int msgId)    );
    MOCK_METHOD1( purge ,     void(const ByteMergerBufferKey& key)        );
    MOCK_METHOD3( tryMerge ,  bool(
                                            std::shared_ptr<ByteArrayPackage>   originalPackage, 
                                            const std::string&                  msgGroupKey,
                                            std::shared_ptr<ByteArrayPackage>&  outPackage
                                        )
                            );
    MOCK_METHOD4( tryMerge ,  bool(
                                            std::shared_ptr<ByteArrayPackage>   originalPackage, 
                                            const std::string&                  msgGroupKey,
                                            std::shared_ptr<ByteArrayPackage>&  outPackage,
                                            ByteMergerBufferKey&                bufferKey
                                        )
                            );

};


TEST(byteMergingModifierTest, Modify_MergeReturnsBytes_ShouldRaisePackageAndReturnCompletedTask) 
{
    // Arrange
    ByteArray bytes = ByteArray::initRandom(500);

    TransportContext transportContext;
    transportContext["Test"] = "123";

    auto package = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    MyMockMerger merger;

    IByteMerger::ByteMergerBufferKey bufferkey( string("123"), 1 );


    ByteMergingModifier modifier(&merger);
    ON_CALL( 
        merger, 
        tryMerge(
            _, 
            _,
            _,
            _
        ) 
    ).WillByDefault(
        DoAll(
            SetArgReferee<2>(package), 
            SetArgReferee<3>(bufferkey),             
            Return(true)
        )
    );


    std::vector<std::shared_ptr<ByteArrayPackage>> nonGeneric;
    modifier.onNewPackage += [&](std::shared_ptr<ByteArrayPackage> package){
        nonGeneric.push_back(package);
    };


    EXPECT_CALL(merger, tryMerge(_,_,_,_));


    modifier.send(package);

    //TODO: update once the modifier.onNewPackage returns async

    ASSERT_EQ( nonGeneric.size(), 1 );
    auto& raisedPackage = nonGeneric[0];
    ASSERT_EQ( *raisedPackage, *package );
}


// define a matcher if ==operator is not needed in production
MATCHER_P(EqByteArrayPackagePtr, other, "Equality matcher for type Foo") {
    return *arg == *other;
}

TEST(byteMergingModifierTest, Modify_SplitPackageMerges_ShouldHaveTransportContextOfFirstPackage) 
{
    // Reason behind this is: When the transport context contains information such as offset the package is from,
    // then if you have following scenario:
    // [Package1_segment1 offset 43] [Package2 offset 44] [package1_segment2 offset 45]
    // then because of what the package raise order is (package1, package2, see other tests), if you were to 
    // assign offset 45 to package 1, commit it and then crash, you would lose out on package 2.
    // Therefore one should instead commit offset 43, so package1 is "handled",
    // (you wouldn't have the segments to re-assamble it again), without losing package 2

    // Arrange
    MyMockMerger merger;

    // P1_s1
    TransportContext transportContext;
    transportContext["Package"] = "1";
    auto bytes = ByteArray::initEmpty(1);
    bytes.data()[0] = 1;
    auto p1s1 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    EXPECT_CALL( 
        merger, 
        purge(
            _
        ) 
    );
    
    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p1s1), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p1s1), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p1")),             
            Return(false)
        )
    );

    // P1_s2
    TransportContext transportContext2;
    transportContext2["Package"] = "1";
    auto bytes2 = ByteArray::initEmpty(1);
    bytes2.data()[0] = 2;
    auto p1s2 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes2, transportContext2)
                    );

    // p1_merged
    TransportContext transportContext3;
    transportContext3["Package"] = "1";
    auto bytes3 = ByteArray::initEmpty(1);
    bytes3.data()[0] = 3;
    auto p1merged = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes3, transportContext3)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p1s2), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p1merged), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p1")),             
            Return(true)
        )
    );




    ByteMergingModifier modifier(&merger);


    std::shared_ptr<ByteArrayPackage> receivedPackage(nullptr);
    modifier.onNewPackage += [&](std::shared_ptr<ByteArrayPackage> package){
        receivedPackage = package;
    };


    // Act
    modifier.send(p1s1);
    modifier.send(p1s2);

    //TODO: update once the modifier.onNewPackage returns async

    // Assert
    ASSERT_NE( receivedPackage.get(), nullptr );
    string ret("");
    receivedPackage->transportContext()->tryGetValue("Package", ret);
    ASSERT_EQ( ret , "1" );
}


TEST(byteMergingModifierTest, Modify_SplitPackageInterweavedWithOtherPackages_ShouldRaiseInExpectedOrder) 
{
    // This is a bit of complex text. The idea is that if you have the following data to be merged:
    // [Package1_segment1] [Package2_segment1] [Package3] [Package1_segment2] [Package2_segment2]
    // then the outcome is [Package1_merged] [Package2_merged] [Package3]

    // Arrange
    MyMockMerger merger;

    EXPECT_CALL( 
        merger, 
        purge(
            _
        ) 
    ).Times(3);




    // P1_s1
    TransportContext transportContext;
    transportContext["Package"] = "1";
    auto bytes = ByteArray::initEmpty(1);
    bytes.data()[0] = 1;
    auto p1s1 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p1s1), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p1s1), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p1")),             
            Return(false)
        )
    );

    // P1_s2
    TransportContext transportContext2;
    transportContext2["Package"] = "1";
    auto bytes2 = ByteArray::initEmpty(1);
    bytes2.data()[0] = 2;
    auto p1s2 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes2, transportContext2)
                    );

    // p1_merged
    TransportContext transportContext3;
    transportContext3["Package"] = "1";
    auto bytes3 = ByteArray::initEmpty(1);
    bytes3.data()[0] = 3;
    auto p1merged = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes3, transportContext3)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p1s2), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p1merged), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p1")),             
            Return(true)
        )
    );




    // P2_s1
    TransportContext transportContext20;
    transportContext20["Package"] = "2";
    auto bytes21 = ByteArray::initEmpty(1);
    bytes21.data()[0] = 4;
    auto p2s1 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes21, transportContext20)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p2s1), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p2s1), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p2")),             
            Return(false)
        )
    );

    // P2_s2
    TransportContext transportContext22;
    transportContext22["Package"] = "2";
    auto bytes22 = ByteArray::initEmpty(1);
    bytes22.data()[0] = 5;
    auto p2s2 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes22, transportContext22)
                    );

    // p2_merged
    TransportContext transportContext2m;
    transportContext2m["Package"] = "2";
    auto bytes2m = ByteArray::initEmpty(1);
    bytes2m.data()[0] = 6;
    auto p2merged = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes2m, transportContext2m)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p2s2), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p2merged), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p2")),             
            Return(true)
        )
    );

    // P3
    TransportContext transportContext30;
    transportContext30["Package"] = "3";
    auto bytes3m = ByteArray::initEmpty(1);
    bytes3m.data()[0] = 7;
    auto p3 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes3m, transportContext30)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p3), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p3), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey()),             
            Return(true)
        )
    );


    ByteMergingModifier modifier(&merger);


    vector<std::shared_ptr<ByteArrayPackage>> expectedOrder;
    expectedOrder.push_back(p1merged);
    expectedOrder.push_back(p2merged);
    expectedOrder.push_back(p3);

    vector<std::shared_ptr<ByteArrayPackage>> packagesReceived;
    modifier.onNewPackage += [&](std::shared_ptr<ByteArrayPackage> package){
        packagesReceived.push_back(package);
    };
    


    // Act
    modifier.send(p1s1);
    auto size1 = packagesReceived.size();
    EXPECT_EQ( size1, 0 );  // only segments so far
    modifier.send(p2s1);
    auto size2 = packagesReceived.size();
    EXPECT_EQ( size2, 0 );  // only segments so far
    modifier.send(p3);
    auto size3 = packagesReceived.size();
    EXPECT_EQ( size3, 0 );  // have a normal package, but in-between segments
    modifier.send(p1s2);
    auto size4 = packagesReceived.size();
    EXPECT_EQ( size4, 1 );  // first package segments arrived, but the normal package is still between segments
    modifier.send(p2s2);
    auto size5 = packagesReceived.size();
    ASSERT_EQ( size5, 3 );  // the last segment arrived for the package wrapping the normal package, so both the normal and the merged release

    // Assert
    for( int i = 0; i < packagesReceived.size(); ++i )
    {
        ASSERT_THAT( packagesReceived[i], EqByteArrayPackagePtr(expectedOrder[i]) );
    }
}




////// TODO: associated with purging

/*
TEST(byteMergingModifierTest, Modify_SplitPackageInterweavedWithOtherAndSplitPackageExpires_ShouldRaiseInExpectedOrder) 
{
    // This is a bit of complex text. The idea is that if you have the following data to be merged:
    // [Package1_segment1] [Package2_segment1] [Package3] [Package1_segment2] [Package2_segment2]
    // then the outcome is [Package1_merged] [Package2_merged] [Package3]


    // Arrange
    MyMockMerger merger;



    // P1_s1
    TransportContext transportContext;
    transportContext["Package"] = "1";
    auto bytes = ByteArray::initEmpty(1);
    bytes.data()[0] = 1;
    auto p1s1 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p1s1), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p1s1), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey("p1")),             
            Return(false)
        )
    );


    // P2
    TransportContext transportContext2;
    transportContext2["Package"] = "3";
    auto bytes2m = ByteArray::initEmpty(1);
    bytes2m.data()[0] = 7;
    auto p2 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes2m, transportContext2)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            EqByteArrayPackagePtr(p2), 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>(p2), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey()),             
            Return(true)
        )
    );



    ByteMergingModifier modifier(&merger);


    vector<std::shared_ptr<ByteArrayPackage>> expectedOrder;
    expectedOrder.push_back(p2);


    vector<std::shared_ptr<ByteArrayPackage>> packagesReceived;
    modifier.onNewPackage += [&](std::shared_ptr<ByteArrayPackage> package){
        packagesReceived.push_back(package);
    };


    // Act
    modifier.send(p1s1);
//    merger.onMessageSegmentsPurged = Raise.Event<Action<string>>("p1"); // IMPORTANT! this is before p2
    merger.onMessageSegmentsPurged = [](const IByteMerger::ByteMergerBufferKey& bufferKey) {}; // IMPORTANT! this is before p2
    modifier.send(p2);

    // Assert
    //TODO: wait till completion of once async / await is implemented
    // tasks.All(x=> x.IsCompleted).Should().BeTrue();

    ASSERT_EQ( packagesReceived.size(), 1 );
    for( int i = 0; i < packagesReceived.size(); ++i )
    {
        ASSERT_THAT( packagesReceived[i], EqByteArrayPackagePtr(expectedOrder[i]) );
    }
}



Modify_SplitPackageInterweavedWithOtherAndSplitPackageExpiresAfterNonSplit_ShouldRaiseInExpectedOrder



*/

TEST(byteMergingModifierTest, Modify_MergeReturnsNull_ShouldNotRaisePackageAndReturnCompletedTask) 
{
    // This is a bit of complex text. The idea is that if you have the following data to be merged:
    // [Package1_segment1] [Package2_segment1] [Package3] [Package1_segment2] [Package2_segment2]
    // then the outcome is [Package1_merged] [Package2_merged] [Package3]

    // Arrange
    MyMockMerger merger;

    // Package
    TransportContext transportContext;
    transportContext["Package"] = "1";
    auto bytes = ByteArray::initEmpty(1);
    bytes.data()[0] = 1;
    auto p1 = std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(bytes, transportContext)
                    );

    EXPECT_CALL( 
        merger, 
        tryMerge(
            _, 
            _,
            _,
            _
        ) 
    ).Times(1).WillRepeatedly(
        DoAll(
            SetArgReferee<2>( std::shared_ptr<ByteArrayPackage>( nullptr ) ), 
            SetArgReferee<3>(IByteMerger::ByteMergerBufferKey()),             
            Return(false)
        )
    );

    std::shared_ptr<ByteArrayPackage> nongeneric( nullptr );

    ByteMergingModifier modifier(&merger);
    modifier.onNewPackage += [&](std::shared_ptr<ByteArrayPackage> package){
        nongeneric = package;
    };


    // Act
    modifier.send(p1);

    // TODO: async await do assert on time elapsed

    ASSERT_EQ( nongeneric.get(), nullptr );
}



/**** 
 TODO: revocation

 Modify_SplitPackageInterweavedWithOtherAndSplitPackageRevoked_ShouldDiscardRevokedAndRaiseInExpectedOrder

****/