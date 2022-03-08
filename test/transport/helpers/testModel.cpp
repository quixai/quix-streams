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



ByteArray TestModelCodec::serialize(const TestModel& value) const{
    const auto len = sizeof(TestModel);
    const auto arr = ByteArray(
        std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
        len
    );
    //serialize in a simple way of only dumping the memory footprint of TestModel into array
    *((TestModel*)arr.data()) = value;

    return arr;
}

TestModel TestModelCodec::deserialize(const ByteArray& barr) const
{
    if(barr.len() < sizeof(TestModel)){
        throw DeserializingException("TOO LITTLE OF DATA");
    }

    //deserialize in a simple way of only casting the input memory array into the TestModel
    TestModel model = *((TestModel*)barr.data());

    return model;
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


TestModel::_init TestModel::_initializer;
TestModelCodec testModelCodec;
Quix::Transport::BaseCodec* TestModel::defaultCodec = &testModelCodec;
