#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "transport/io/ISubscriber.h"
#include "transport/fw/ICanCommit.h"

#include "transport/codec/binaryCodec.h"
#include "transport/codec/ICodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/io/package.h"
#include "transport/io/IModel.h"
#include "transport/transportSubscriber.h"
#include "transport/transportPublisher.h"

#include "./helpers/passthrough.h"
#include "./helpers/packageFactory.h"

#include <cstring>
#include <algorithm>

using namespace std;
using namespace Quix::Transport;


using ::testing::_;
using ::testing::Return;

///TODO: Commiting & Revoking tests

class TestSingleMessage : public IModel{
    static const size_t MSG_SIZE = 100;
public:
    char msg[MSG_SIZE];

    TestSingleMessage( const char* msg = "" ) { 
        strncpy(this->msg, msg, MSG_SIZE - 1); 
        this->msg[MSG_SIZE - 1] = '\0';
    };

    bool operator==(const TestSingleMessage& other) const {
        return strcmp(msg, other.msg) == 0;
    };

    const ModelKey modelKey() const {
        return Quix::Transport::ModelKey::forType<TestSingleMessage>();
    };

};

TEST(transportWhole, TransportOutput_ShouldCorrespondToTransportInput)
{
    // This test checks that Transport Input and Output are reverse of each other

    static BinaryCodec<TestSingleMessage> codec1;
    auto registry = Quix::Transport::CodecRegistry::instance();
    registry->registerCodec( Quix::Transport::ModelKey::forType<TestSingleMessage>() , &codec1 );

    // Arrange
    Passthrough passthrough;
    ByteSplitter byteSplitter(15);  // this tiny to force some splitting
    TransportPublisher transportPublisher(&passthrough, &byteSplitter);
    TransportSubscriber transportSubscriber(&passthrough);

    TestSingleMessage sentValue("TEST MESSAGE");

    std::shared_ptr<IPackage> sentPackage(
        new Package<TestSingleMessage>(
            sentValue
        )
    );

    vector<std::shared_ptr<IPackage>> packagesReceived;
    transportSubscriber.onNewPackage += [&](std::shared_ptr<IPackage> package){
        packagesReceived.push_back( package );
    };


    // Act
    transportPublisher.send(sentPackage);

    // Assert
    EXPECT_EQ( packagesReceived.size(), 1 );

    std::shared_ptr<Package<TestSingleMessage>> received(nullptr);
    EXPECT_NE( (received = dynamic_pointer_cast<Package<TestSingleMessage>>(packagesReceived[0]) ).get(), nullptr );
    EXPECT_EQ( received->value(), sentValue );

}

