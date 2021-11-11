#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"
#include "transport/fw/exceptions/deserializingException.h"
#include "transport/fw/exceptions/serializingException.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

class MockCodec : public AbstractCodec
{
    private:
    public:
        MockCodec(const std::string& id): AbstractCodec(id){};
        ByteArray serialize(const std::shared_ptr<IPackage> obj) const{
            throw SerializingException("UNREACHABLE");
        };
        const std::shared_ptr<IPackage> deserialize(const std::shared_ptr<ByteArrayPackage> package) const{
            throw DeserializingException("UNREACHABLE");
        };

};

TEST(codecRegistryTest, shouldRegisterCodec)
{
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");
    registry->registerCodec(modelKey, &codec1);

    // Act
    auto retrievedCodec = registry->retrieveCodec(modelKey, codec1.codecId());

    // Assert
    EXPECT_EQ (codec1.codecId(),  "TestCodec");
    EXPECT_EQ (retrievedCodec,  &codec1);
}

TEST(codecRegistryTest, hasPreviousRegistration_ShouldReturnRegisteredCodec)
{
    //arrange
    MockCodec codec1("TestCodec1");
    MockCodec codec2("TestCodec2");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Test");
    registry->registerCodec(modelKey, &codec1);
    registry->registerCodec(modelKey, &codec2);

    // Act
    auto retrievedCodec = registry->retrieveCodec(modelKey, "TestCodec1");
    auto retrievedCodec2 = registry->retrieveCodec(modelKey, "TestCodec2");
    auto allCodecs = registry->retrieveCodecs(modelKey);

    // Assert
    EXPECT_EQ ( retrievedCodec, &codec1 );
    EXPECT_EQ ( retrievedCodec2, &codec2 );
    EXPECT_EQ ( allCodecs.size(),  2 );

    auto it = allCodecs.begin();
    EXPECT_EQ ( *it++, &codec1 );
    EXPECT_EQ ( *it++, &codec2 );
}

TEST(codecRegistryTest, hasPreviousRegistration_ShouldReturnNull)
{
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");
    registry->registerCodec(modelKey, &codec1);

    // Act
    registry->clearCodecs(modelKey);
    auto retrievedCodec = registry->retrieveCodec(modelKey, codec1.codecId());
    auto allCodecs = registry->retrieveCodecs(modelKey);

    // Assert
    EXPECT_EQ ( retrievedCodec, nullptr );
    EXPECT_TRUE ( allCodecs.empty() );
}

TEST(codecRegistryTest, validCodec_ShouldAlsoRegisterInModelKeyRegistry)
{
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");

    // Act
    registry->registerCodec(modelKey, &codec1);

    ModelKey outModelKey("");
    bool typeFound = ModelKeyRegistry::instance()->tryGetModelKey(codec1.codecId(), outModelKey);

    // Assert
    ASSERT_TRUE ( typeFound );
    EXPECT_EQ ( outModelKey, modelKey );
}
