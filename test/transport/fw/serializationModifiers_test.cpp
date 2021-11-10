#include "gtest/gtest.h"

#include "transport/io/TransportContext.h"
#include "transport/io/Package.h"
#include "transport/fw/deserializingModifier.h"
#include "transport/fw/serializingModifier.h"

#include "../helpers/testModel.h"

#include <memory>
#include <algorithm>
#include <map>
#include <string>

using namespace std;
using namespace Quix::Transport;

TEST(transportContextTest, WithSingleObject_ShouldCorrespondToSerializingModifier) {
    // This test checks that DeserializingModifier and SerializingModifier are reverse of each other

    // Arrange
    TestModel testModel;
    MetaData metaData;
    metaData[std::string("Key1")] = std::string("Value1");
    metaData[std::string("Key2")] = std::string("Value2");

    shared_ptr<Package<TestModel>> packagePtr(new Package<TestModel>(testModel, metaData));
    DeserializingModifier deserializingModifier;
    SerializingModifier serializingModifier;

    serializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializingModifier.send(package);
            };
    
    shared_ptr<IPackage> deserializedPackage(nullptr);

    deserializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializedPackage = package;
            };
    
    // Act
    serializingModifier.send(packagePtr);

    // Assert
    ASSERT_NE( deserializedPackage.get(), nullptr );

    shared_ptr<Package<TestModel>> deserializedPackageTyped(nullptr);
    EXPECT_NO_THROW( deserializedPackageTyped = std::dynamic_pointer_cast<Package<TestModel>>(deserializedPackage) );
    ASSERT_NE( deserializedPackageTyped.get(), nullptr );
    EXPECT_EQ( *deserializedPackageTyped, *packagePtr );
}

TEST(transportContextTest, WithQuixByteArray_ShouldCorrespondToSerializingModifier) {
    // This test checks that DeserializingModifier and SerializingModifier are reverse of each other

    // Arrange
    ByteArray testModel = ByteArray::initEmpty(100);
    MetaData metaData;
    metaData[std::string("Key2")] = std::string("Value2");

    shared_ptr<Package<ByteArray>> packagePtr(new Package<ByteArray>(testModel, metaData));
    DeserializingModifier deserializingModifier;
    SerializingModifier serializingModifier;

    serializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializingModifier.send(package);
            };
    
    shared_ptr<IPackage> deserializedPackage(nullptr);

    deserializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializedPackage = package;
            };
    
    // Act
    serializingModifier.send(packagePtr);

    // Assert
    ASSERT_NE( deserializedPackage.get(), nullptr );

    shared_ptr<Package<ByteArray>> deserializedPackageTyped(nullptr);
    EXPECT_NO_THROW( deserializedPackageTyped = std::dynamic_pointer_cast<Package<ByteArray>>(deserializedPackage) );
    ASSERT_NE( deserializedPackageTyped.get(), nullptr );
    EXPECT_EQ( *deserializedPackageTyped, *packagePtr );
}

TEST(transportContextTest, WithStringArray_ShouldCorrespondToSerializingModifier) {
    // This test checks that DeserializingModifier and SerializingModifier are reverse of each other

    // Arrange
    ByteArray testModel(string("TEST_STRING_12412315345"));
    MetaData metaData;
    metaData[std::string("Key1")] = std::string("Value1");

    shared_ptr<Package<ByteArray>> packagePtr(new Package<ByteArray>(testModel, metaData));
    DeserializingModifier deserializingModifier;
    SerializingModifier serializingModifier;

    serializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializingModifier.send(package);
            };
    
    shared_ptr<IPackage> deserializedPackage(nullptr);

    deserializingModifier.onNewPackage = [&](shared_ptr<IPackage> package)
            {
                deserializedPackage = package;
            };
    
    // Act
    serializingModifier.send(packagePtr);

    // Assert
    ASSERT_NE( deserializedPackage.get(), nullptr );

    shared_ptr<Package<ByteArray>> deserializedPackageTyped(nullptr);
    EXPECT_NO_THROW( deserializedPackageTyped = std::dynamic_pointer_cast<Package<ByteArray>>(deserializedPackage) );
    ASSERT_NE( deserializedPackageTyped.get(), nullptr );
    EXPECT_EQ( *deserializedPackageTyped, *packagePtr );
}


