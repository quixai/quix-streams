#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "transport/io/ISubscriber.h"
#include "transport/fw/ICanCommit.h"

#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/io/package.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "./helpers/passthrough.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;


using ::testing::_;

///TODO: Commiting & Revoking tests



class MyMockOutput : public ISubscriber, public ICanCommit {

 public:
    MOCK_METHOD1( commit ,    void(const std::vector<std::shared_ptr<TransportContext>>& transportContexts) );
    MOCK_METHOD2( filterCommittedContexts ,    std::vector<std::shared_ptr<TransportContext>>(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter) );

};


TEST(transportSubscriber, Commit_WithTransport_ShouldAckOutput)
{
    // Arrange
    MyMockOutput output;
    bool received_commited = false;
    std::vector<std::shared_ptr<TransportContext>> commited;


    ON_CALL( output, commit(_) ).WillByDefault(
        testing::Invoke([&]( const std::vector<std::shared_ptr<TransportContext>>& transportContexts ) {
            received_commited = true;
            commited = transportContexts;
        })
    );

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


TEST(transportSubscriber, Setup_OutputRaisesEnoughForAutoCommitToKickIn_ShouldCommitIntoOutput)
{
    // Arrange
    Passthrough output;

    bool received = false;
    ICanCommit::OnCommittedEventArgs committed;
    output.onCommitted += [&](ICanCommit* sender, const ICanCommit::OnCommittedEventArgs& args){ received = true; committed = args; };

    // TransportOutput transportOutput(&output, o=> o.CommitOptions.CommitEvery = 1);
    
    std::shared_ptr<TransportContext> transportContext( new TransportContext() );
    (*transportContext)["abcde"] = "1";

    // var package = PackageFactory.CreatePackage(new {TestMessage = "YAY"}, transportContext);
    
    // // Act
    // output.OnNewPackage.Invoke(package);

    // // Assert
    // committed.Should().NotBeNull();
    // committed.State.Should().BeEquivalentTo(new[] {transportContext}); // PassThrough sends back the transport context as state
   
}
