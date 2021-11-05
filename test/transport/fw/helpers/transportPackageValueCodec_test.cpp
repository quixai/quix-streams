#include "gtest/gtest.h"
#include "transport/fw/helpers/transportPackageValueCodec.h"
#include "transport/io/rawBytePackageValue.h"

#include <algorithm>
#include <memory>

using namespace std;
using namespace Quix::Transport;

TEST(transportPackageValueCodec, simpleTest) {
    //arrange
    ModelKey modelKey("modelKey");

    RawBytePackage* package = new RawBytePackage(modelKey, RawBytePackageValue::initEmpty(30), MetaData());

    // Act
    RawBytePackageValue raw(TransportPackageValueCodec::Serialize(package));
    RawBytePackage* deserializedPackage = TransportPackageValueCodec::Deserialize(raw);

    // Assert
    
    ASSERT_EQ( *deserializedPackage, *package );

    // Cleanup
    delete deserializedPackage;
    delete package;

}

