#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/registry/modelKeyRegistry.h"
#include "transport/transport.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

class MockCodec : public AbstractCodec
{
    private:
    public:
        MockCodec(const std::string& id): AbstractCodec(id){};
};

TEST(modelKeyRegistryTest, retrieve_AfterSuccessfulRegister_ShouldReturnExpected)
{
    // Arrange
    ModelKey modelKey("TESTModel");
    auto codecKey = std::string("TESTCodec");

    auto registry = ModelKeyRegistry::instance();
    registry->registerModel(codecKey, modelKey);


    // Act
    ModelKey outModelKey("___TESTERROR___");
    auto retrievedModelKeySuccess = registry->tryGetModelKey(codecKey, outModelKey);

    // // Assert
    ASSERT_TRUE ( retrievedModelKeySuccess );
    ASSERT_EQ ( outModelKey, modelKey );
}

TEST(modelKeyRegistryTest, retrieve_AfterSuccessfulRegister_ShouldNotFound)
{
    // Arrange
    ModelKey modelKey("TESTModel");
    ModelKey modelKey2("TESTModel2");
    auto codecKey = std::string("TESTCodec");
    auto codecKey2 = std::string("TESTCodec2");

    auto registry = ModelKeyRegistry::instance();
    registry->registerModel(codecKey, modelKey);


    // Act
    ModelKey outModelKey("___TESTERROR___");
    auto retrievedModelKeySuccess = registry->tryGetModelKey(codecKey2, outModelKey);

    // // Assert
    ASSERT_FALSE ( retrievedModelKeySuccess );
    ASSERT_EQ ( outModelKey, std::string("___TESTERROR___") );
}

