#include "gtest/gtest.h"
#include "transport/fw/helpers/transportPackageValueCodec.h"
#include "transport/io/rawBytePackageValue.h"

#include <algorithm>

using namespace std;
using namespace Quix::Transport;

TEST(transportPackageValueCodec, simpleTest) {
    //arrange
    ModelKey modelKey("modelKey");
    RawBytePackage* package = new RawBytePackage(modelKey, RawBytePackageValue(new uint8_t[30], 30), MetaData());

    // Act
    RawBytePackageValue raw(TransportPackageValueCodec::Serialize(package));
    RawBytePackage* deserializedPackage = TransportPackageValueCodec::Deserialize(raw);

    // Assert
    
    ASSERT_EQ( *deserializedPackage, *package );

    // Cleanup
    delete raw.data();

    delete deserializedPackage->value().data();
    delete deserializedPackage;

    delete package->value().data();
    delete package;

}

