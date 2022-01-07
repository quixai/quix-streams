#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "transport/io/ISubscriber.h"
#include "transport/fw/ICanCommit.h"

#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
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

///TODO: Commiting & Revoking tests



class MyMockCommitOutput : public ISubscriber, public ICanCommit {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterCommittedContexts ,    std::vector<std::shared_ptr<TransportContext>>(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) );

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
    MOCK_METHOD2( filterRevokedContexts ,    std::vector<std::shared_ptr<TransportContext>>(void* state, std::vector<std::shared_ptr<TransportContext>> contexts) );

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
    EXPECT_CALL(output, commit(_));

    // Act
    IRevocationPublisher::OnRevokingEventArgs expectedArgs;

    // .Net equivalent of output.OnRevoking += Raise.eventWith
    output.onRevoking(&output, expectedArgs);

    // Assert
    ASSERT_EQ( revokingInvokes.size(), 1 );
    ASSERT_EQ( revokingInvokes[0], expectedArgs );
}
