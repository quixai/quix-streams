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
    inline RawBytePackageValue(const std::shared_ptr<uint8_t> data, size_t len, size_t start = 0) : len_(len), start_(start), data_(data) {};
    inline RawBytePackageValue(const std::string& data) :
        len_(data.size()),
        start_(0),
        data_(std::shared_ptr<uint8_t>(new uint8_t[len_], std::default_delete<uint8_t[]>())) 
    {
        memcpy((void*)&(*data_), data.c_str(), len_);
    };
    inline RawBytePackageValue() : len_(0), start_(0), data_(nullptr) {};
    inline RawBytePackageValue(const RawBytePackageValue& other) = default;
    inline RawBytePackageValue(const RawBytePackageValue& other, size_t from, size_t len) : RawBytePackageValue(other.rawdata(), len, from) {};
    static inline RawBytePackageValue initEmpty(size_t len)
    {
        return RawBytePackageValue(
            std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
            len
        );
    };
    static inline RawBytePackageValue prependHeader(uint8_t* headerArray, size_t headerLength, const RawBytePackageValue& rest) {
        const auto restLen = rest.len();

        auto array = RawBytePackageValue::initEmpty(headerLength + restLen);
        memcpy(&*array.data(), headerArray, headerLength);
        memcpy(&*array.data() + headerLength, rest.data(), restLen);
        return array;
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
    inline uint8_t*const begin() const { return &(*data_) + start_; }
    inline const std::shared_ptr<uint8_t> rawdata() const { return data_; }
};

} }