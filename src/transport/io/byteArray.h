#pragma once

#include <cstring>
#include <memory>

#include "../fw/modelKey.h"
#include "./IModel.h"

namespace Quix { namespace Transport {

//Hacky solution to contain array with length
class ByteArray : public IModel{
private:
    const size_t len_;
    const size_t start_;
    const std::shared_ptr<uint8_t> data_;

public:
    inline ByteArray(const std::shared_ptr<uint8_t> data, size_t len, size_t start = 0) : len_(len), start_(start), data_(data) {};
    inline ByteArray(const std::string& data) :
        len_(data.size()),
        start_(0),
        data_(std::shared_ptr<uint8_t>(new uint8_t[len_], std::default_delete<uint8_t[]>())) 
    {
        memcpy((void*)&(*data_), data.c_str(), len_);
    };
    inline ByteArray() : len_(0), start_(0), data_(nullptr) {};
    inline ByteArray(const ByteArray& other) = default;
    inline ByteArray(const ByteArray& other, size_t from, size_t len) : ByteArray(other.rawdata(), len, from) {};
    static inline ByteArray initEmpty(size_t len)
    {
        return ByteArray(
            std::shared_ptr<uint8_t>(new uint8_t[len], std::default_delete<uint8_t[]>()),
            len
        );
    };
    static inline ByteArray prependHeader(uint8_t* headerArray, size_t headerLength, const ByteArray& rest) {
        const auto restLen = rest.len();

        auto array = ByteArray::initEmpty(headerLength + restLen);
        memcpy(&*array.data(), headerArray, headerLength);
        memcpy(&*array.data() + headerLength, rest.data(), restLen);
        return array;
    };
    inline const ModelKey modelKey() const { return ModelKey("RawBytePackage"); };
    inline const bool operator==(const ByteArray& other) const { 
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