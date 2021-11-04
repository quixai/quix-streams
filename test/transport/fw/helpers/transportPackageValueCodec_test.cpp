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

    const std::shared_ptr<uint8_t> data(new uint8_t[30], std::default_delete<uint8_t[]>()); 
    RawBytePackage* package = new RawBytePackage(modelKey, RawBytePackageValue(data, 30), MetaData());

    // Act
    RawBytePackageValue raw(TransportPackageValueCodec::Serialize(package));
    RawBytePackage* deserializedPackage = TransportPackageValueCodec::Deserialize(raw);

    // Assert
    
    ASSERT_EQ( *deserializedPackage, *package );

    // Cleanup
    delete deserializedPackage;
    delete package;

}

