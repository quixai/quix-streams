#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"

#include <algorithm>

using namespace std;
using namespace Quix;

class MockCodec : public AbstractCodec{
    private:
    public:
        MockCodec(const std::string& id): AbstractCodec(id){};
};

TEST(codecRegistryTest, shouldRegisterCodec) {
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");
    registry->registerCodec(modelKey, &codec1);

    // Act
    auto retrievedCodec = registry->retrieveCodec(modelKey, codec1.key());

    // Assert
    EXPECT_EQ (codec1.key(),  "TestCodec");
    EXPECT_EQ (retrievedCodec,  &codec1);
}

TEST(codecRegistryTest, hasPreviousRegistration_ShouldReturnRegisteredCodec) {
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

TEST(codecRegistryTest, hasPreviousRegistration_ShouldReturnNull) {
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");
    registry->registerCodec(modelKey, &codec1);

    // Act
    registry->clearCodecs(modelKey);
    auto retrievedCodec = registry->retrieveCodec(modelKey, codec1.key());
    auto allCodecs = registry->retrieveCodecs(modelKey);

    // Assert
    EXPECT_EQ ( retrievedCodec, nullptr );
    EXPECT_TRUE ( allCodecs.empty() );
}

TEST(codecRegistryTest, validCodec_ShouldAlsoRegisterInModelKeyRegistry) {
    //arrange
    MockCodec codec1("TestCodec");

    auto registry = CodecRegistry::instance();
    ModelKey modelKey("Model1");

    // Act
    registry->registerCodec(modelKey, &codec1);

    std::string codecKey("");
    bool type_found = ModelKeyRegistry::instance()->tryGetCodecKey(modelKey, codecKey);

    // Assert
    ASSERT_TRUE ( type_found );
    EXPECT_EQ ( codecKey, codec1.key() );
}
