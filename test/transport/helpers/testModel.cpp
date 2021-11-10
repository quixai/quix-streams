#include "./testModel.h"

#include <iostream>

using namespace std;
using namespace Quix::Transport;

bool TestModel::operator==(const TestModel& other) const {
    return true;
};



TestModelCodec::TestModelCodec(): AbstractCodec(CodecId("TestModelCodec")){};

ByteArray TestModelCodec::serialize(const std::shared_ptr<IPackage> obj) const{
    const auto len = 0;
    return ByteArray(
        std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
        len
    );
}

const std::shared_ptr<IPackage> TestModelCodec::deserialize(const std::shared_ptr<RawBytePackage> rawPackage) const{
    return std::shared_ptr<IPackage>(
        new Package<TestModel>(rawPackage, TestModel()),
        std::default_delete<IPackage>()
    );
}


TestModelCodec TestModel::defaultCodec;
TestModel::_init TestModel::_initializer;