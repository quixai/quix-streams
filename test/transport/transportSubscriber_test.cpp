#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "transport/io/ISubscriber.h"
#include "transport/fw/ICanCommit.h"

#include "transport/codec/ICodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/io/package.h"
#include "transport/io/IModel.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "./helpers/passthrough.h"
#include "./helpers/packageFactory.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;


using ::testing::_;
using ::testing::Return;



class MyMockCommitOutput : public ISubscriber, public ICanCommit {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterCommittedContexts ,    std::vector<std::shared_ptr<TransportContext>>(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) );

};


TEST(transportSubscriber, Commit_WithTransport_ShouldAckOutput)
{
    // Arrange
    MyMockCommitOutput output;
    bool received_commited = false;
    std::vector<std::shared_ptr<TransportContext>> commited;


    ON_CALL( output, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            received_commited = true;
            commited = transportContexts;
        })
    );
    EXPECT_CALL(output, commit(_));

    TransportSubscriber transportSubscriber(&output);
    
    std::shared_ptr<TransportContext> transportContext( new TransportContext() );
    (*transportContext)["abcde"] = "1";

    std::shared_ptr<TransportContext> transportContext2( new TransportContext() );
    (*transportContext)["abcdef"] = "2";
    
    // Act
    std::vector<std::shared_ptr<TransportContext>> toCommit;
    toCommit.push_back(transportContext);
    toCommit.push_back(transportContext2);

    transportSubscriber.commit(toCommit);



    // Assert
    EXPECT_TRUE( received_commited );


    ASSERT_EQ( commited.size() , toCommit.size() );
    for( int i = 0; i < commited.size(); ++i )
    {
        EXPECT_EQ( *(commited[i]), *(toCommit[i]) );
    }
}


class TestMessage : public IModel{
    static const size_t MSG_SIZE = 100;
public:
    char msg[MSG_SIZE];

    TestMessage( const char* msg = "" ) { 
        strncpy(this->msg, msg, MSG_SIZE - 1); 
        this->msg[MSG_SIZE - 1] = '\0';
    }

    const ModelKey modelKey() const {
        return Quix::Transport::ModelKey::forType<TestMessage>();
    };

};

TEST(transportSubscriber, Setup_OutputRaisesEnoughForAutoCommitToKickIn_ShouldCommitIntoOutput)
{
    // Arrange
    Passthrough output;

    bool received_commited = false;
    ICanCommit::OnCommittedEventArgs committed;
    output.onCommitted += [&](ICanCommit* sender, const ICanCommit::OnCommittedEventArgs& args){ received_commited = true; committed = args; };

    TransportSubscriber transportOutput(&output, []( TransportSubscriberOptions& o ){ o.commitOptions.commitEvery = 1; } );
    
    std::shared_ptr<TransportContext> transportContext( new TransportContext() );
    (*transportContext)["abcde"] = "1";

    auto package = createPackage<TestMessage>( TestMessage("MESSAGE") , transportContext );
    
    /// Act
    output.onNewPackage(package);

    ASSERT_TRUE( received_commited );

    // TODO: Here would be some weird casting via transportContext

    // ASSERT_EQ( commited.state() );
    // committed.State.Should().BeEquivalentTo(new[] {transportContext}); // PassThrough sends back the transport context as state
   
}


class MyMockRevocationOutput : public ISubscriber, public IRevocationPublisher {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterRevokedContexts ,    std::vector<std::shared_ptr<TransportContext>>(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contexts) );

};


TEST(transportSubscriber, Setup_OutputRaisesRevokingEvent_ShouldAlsoRaiseRevokingEvent)
{
    // Arrange
    MyMockRevocationOutput output;
    TransportSubscriber transportSubscriber(&output);
    std::vector<IRevocationPublisher::OnRevokingEventArgs> revokingInvokes;

    transportSubscriber.onRevoking += [&](IRevocationPublisher* sender, const IRevocationPublisher::OnRevokingEventArgs& args)
            {
                revokingInvokes.push_back(args);
            };

    // Act
    IRevocationPublisher::OnRevokingEventArgs expectedArgs;

    // .Net equivalent of output.OnRevoking += Raise.eventWith
    output.onRevoking(&output, expectedArgs);

    // Assert
    ASSERT_EQ( revokingInvokes.size(), 1 );
    ASSERT_EQ( revokingInvokes[0], expectedArgs );
}


TEST(transportSubscriber, Setup_OutputRaisesRevokedEvent_ShouldAlsoRaiseRevokedEvent)
{
    // Arrange
    MyMockRevocationOutput output;
    TransportSubscriber transportSubscriber(&output);
    std::vector<IRevocationPublisher::OnRevokedEventArgs> revokedInvokes;

    transportSubscriber.onRevoked += [&](IRevocationPublisher* sender, const IRevocationPublisher::OnRevokedEventArgs& args)
            {
                revokedInvokes.push_back(args);
            };

    // Act
    IRevocationPublisher::OnRevokedEventArgs expectedArgs;

    // .Net equivalent of output.OnRevoking += Raise.eventWith
    output.onRevoked(&output, expectedArgs);

    // Assert
    ASSERT_EQ( revokedInvokes.size(), 1 );
    ASSERT_EQ( revokedInvokes[0], expectedArgs );
}

TEST(transportSubscriber, Setup_OutputSupportsRevocationPublisher_FilterRevokedContextsShouldBeSameAsOutputs)
{
    // As of writing this test, No other modifier within TransportOutput supports IRevocationFilter,
    // meaning if in future this changes, this test will have to be updated to whatever the new logic is.
    // For now it is assumed that the provided output might implement IRevocationPublisher
    // Arrange

    // Arrange
    MyMockRevocationOutput output;
    TransportSubscriber transportSubscriber(&output);
    
    std::vector<std::shared_ptr<TransportContext>> expectedResult;

    auto tc = new TransportContext();
    (*tc)["abcde"] = "1";
    expectedResult.push_back( std::shared_ptr<TransportContext>( tc ) );

    EXPECT_CALL( output, filterRevokedContexts(_, _) )
        .Times(1).WillRepeatedly(
            Return(expectedResult)
        );

    // Act
    auto result = transportSubscriber.filterRevokedContexts( nullptr, std::vector<std::shared_ptr<TransportContext>>() );

    // Assert
    ASSERT_EQ( result.size(), expectedResult.size() );
    for( size_t i = 0; i < result.size(); ++i ) 
    { 
        ASSERT_EQ( result[i], expectedResult[i] );
    }
    
}

class MyMockCanCommitOutput : public ISubscriber, public ICanCommit {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterCommittedContexts ,    std::vector<std::shared_ptr<TransportContext>>(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) );

};


TEST(transportSubscriber, Setup_OutputRaisesCommitted_ShouldAlsoRaiseCommitted)
{
    // Arrange
    MyMockCanCommitOutput output;
    std::vector<ICanCommit::OnCommittedEventArgs> committed;

    TransportSubscriber transportSubscriber(&output, []( TransportSubscriberOptions& o ){ o.commitOptions.commitEvery = 1; });
    transportSubscriber.onCommitted += [&](ICanCommit* sender, const ICanCommit::OnCommittedEventArgs& args) {
        committed.push_back(args);
    };

    // Act
    ICanCommit::OnCommittedEventArgs eArgs;
    output.onCommitted(&output, eArgs);

    // Assert
    std::vector<ICanCommit::OnCommittedEventArgs> expectedResult;
    expectedResult.push_back(eArgs);

    // Assert
    ASSERT_EQ( committed.size(), expectedResult.size() );
    for( size_t i = 0; i < committed.size(); ++i ) 
    { 
        ASSERT_EQ( committed[i], expectedResult[i] );
    }
};

TEST(transportSubscriber, Setup_OutputRaisesCommitting_ShouldAlsoRaiseCommitting)
{
    // Arrange
    MyMockCanCommitOutput output;
    std::vector<ICanCommit::OnCommittingEventArgs> committing;

    TransportSubscriber transportSubscriber(&output, []( TransportSubscriberOptions& o ){ o.commitOptions.commitEvery = 1; });
    transportSubscriber.onCommitting += [&](ICanCommit* sender, const ICanCommit::OnCommittingEventArgs& args) {
        committing.push_back(args);
    };

    // Act
    ICanCommit::OnCommittingEventArgs eArgs;
    output.onCommitting(&output, eArgs);

    // Assert
    std::vector<ICanCommit::OnCommittingEventArgs> expectedResult;
    expectedResult.push_back(eArgs);

    // Assert
    ASSERT_EQ( committing.size(), expectedResult.size() );
    for( size_t i = 0; i < committing.size(); ++i ) 
    { 
        ASSERT_EQ( committing[i], expectedResult[i] );
    }
};

