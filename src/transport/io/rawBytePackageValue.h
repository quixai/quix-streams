#pragma once

#include <cstring>
#include <memory>

namespace Quix { namespace Transport {

//Hacky solution to contain array with length
class RawBytePackageValue{
private:
    const size_t len_;
    const size_t start_;
    const std::shared_ptr<uint8_t> data_;

public:
    RawBytePackageValue(const std::shared_ptr<uint8_t> data, size_t len, size_t start = 0) : len_(len), start_(start), data_(data) {};
    RawBytePackageValue(const std::string& data) :
        len_(data.size()),
        start_(0),
        data_(std::shared_ptr<uint8_t>(new uint8_t[len_], std::default_delete<uint8_t[]>())) 
    {
        memcpy((void*)&(*data_), data.c_str(), len_);
    };
    RawBytePackageValue() : len_(0), start_(0), data_(nullptr) {};
    RawBytePackageValue(const RawBytePackageValue& other) = default;
    RawBytePackageValue(const RawBytePackageValue& other, size_t from, size_t len) : RawBytePackageValue(other.rawdata(), len, from) {};
    static RawBytePackageValue initEmpty(size_t len)
    {
        return RawBytePackageValue(
            std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
            len
        );
    };
    inline const bool operator==(const RawBytePackageValue& other) const { 
        return 
            len_ == other.len() 
                && 
            memcmp(data(), other.data(), len_) == 0
            ;
    }
    inline const size_t len() const { return len_; }
    inline uint8_t*const data() const { return &(*data_) + start_; }
    inline const std::shared_ptr<uint8_t> rawdata() const { return data_; }
};

} }