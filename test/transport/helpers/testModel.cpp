#include "./testModel.h"

#include "transport/fw/exceptions/deserializingException.h"


#include <cstdlib>     /* srand, rand */
#include <climits>      /* UINT_MAX */

#include <iostream>

using namespace std;
using namespace Quix::Transport;

bool TestModel::operator==(const TestModel& other) const 
{
    return true;
};



TestModelCodec::TestModelCodec()
 : 
 AbstractCodec(CodecId("TestModelCodec"))
{

};

ByteArray TestModelCodec::serialize(const std::shared_ptr<IPackage> obj) const
{
    const auto typedPackage = std::dynamic_pointer_cast<Package<TestModel>>(obj);

    const auto len = sizeof(TestModel);
    const auto arr = ByteArray(
        std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
        len
    );
    //serialize in a simple way of only dumping the memory footprint of TestModel into array
    *((TestModel*)arr.data()) = typedPackage->value();

    return arr;
}

const std::shared_ptr<IPackage> TestModelCodec::deserialize(const std::shared_ptr<ByteArrayPackage> rawPackage) const
{
    ByteArray barr = rawPackage->value();
    if(barr.len() < sizeof(TestModel)){
        throw DeserializingException("TOO LITTLE OF DATA");
    }

    //deserialize in a simple way of only casting the input memory array into the TestModel
    TestModel model = *((TestModel*)barr.data());

    return std::shared_ptr<IPackage>(
        new Package<TestModel>(rawPackage, model),
        std::default_delete<IPackage>()
    );
}

TestModel::TestModel(){
    number_ = rand() % UINT_MAX;

    //random string containing a-z characters of length 9
    int i = 0;
    for(i = 0; i < 9; ++i){
        datastr_[i] = (rand() % ('z' - 'a') + 'a');
    }
    datastr_[i] = 0;
}


TestModelCodec TestModel::defaultCodec;
TestModel::_init TestModel::_initializer;