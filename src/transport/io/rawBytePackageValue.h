#pragma once

#include <cstring>
#include <memory>

namespace Quix { namespace Transport {

//Hacky solution to contain array with length
class RawBytePackageValue{
private:
    const size_t len_;
    const uint8_t* data_;
public:
    RawBytePackageValue(uint8_t* data, size_t len) : len_(len), data_(data) {};
    RawBytePackageValue() : len_(0), data_(nullptr) {};
    RawBytePackageValue(const RawBytePackageValue& other) = default;
    inline const bool operator==(const RawBytePackageValue& other) const { 
        return 
            len_ == other.len() 
                && 
            memcmp(data_, other.data(), len_) == 0
            ;
    }
    inline const size_t len() const { return len_; }
    inline const uint8_t* data() const { return data_; }
};

} }