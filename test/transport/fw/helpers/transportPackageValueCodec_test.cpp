#include "gtest/gtest.h"
#include "transport/fw/helpers/transportPackageValueCodec.h"
#include "transport/io/rawBytePackageValue.h"

#include <algorithm>
#include <memory>

using namespace std;
using namespace Quix::Transport;

void testDeserialization(size_t size){
    //arrange
    ModelKey modelKey("modelKey");

    RawBytePackage* package = new RawBytePackage(modelKey, RawBytePackageValue::initEmpty(size), MetaData());

    // Act
    RawBytePackageValue raw(TransportPackageValueCodec::Serialize(package));
    RawBytePackage* deserializedPackage = TransportPackageValueCodec::Deserialize(raw);

    // Assert
    
    ASSERT_EQ( *deserializedPackage, *package );

    // Cleanup
    delete deserializedPackage;
    delete package;
}

TEST(transportPackageValueCodec, simpleTest) {
    testDeserialization(0);
}

TEST(transportPackageValueCodec, bigger) {
    for(int i = 100; i < 20*100; i+=100){
        testDeserialization(i);
    }
}


