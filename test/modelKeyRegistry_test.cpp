#include "gtest/gtest.h"
#include "transport/codec/abstractCodec.h"
#include "transport/registry/codecRegistry.h"
#include "transport/transport.h"

#include <algorithm>

using namespace std;
using namespace Quix;

class MockCodec : public AbstractCodec{
    private:
        std::string _id;
    public:
        MockCodec(const std::string& id): _id(id){};
        virtual std::string id(){ return _id; };
};

TEST(modelKeyRegistryTest, retrieve_AfterSuccessfulRegister_ShouldReturnExpected) {
    // Arrange
    // auto type = std::type_index(typeid(ModelKeyRegistryShould));
    // var modelKey = new ModelKey(type, 32);
    // ModelKeyRegistry.RegisterModel(type, modelKey);

    // // Act
    // var retrievedType = ModelKeyRegistry.GetType(modelKey);
    // var retrievedModelKey = ModelKeyRegistry.GetModelKey(type);

    // // Assert
    // retrievedType.Should().Be(type);
    // retrievedModelKey.Should().Be(modelKey);

}

