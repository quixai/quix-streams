#pragma once

#include <memory>
#include <vector>

#include "../fw/modelKey.h"

namespace Quix { namespace Transport {

class AbstractPackage{
public:
    virtual ModelKey modelKey() const = 0;
    virtual const void* dataptr() const = 0;
};

template <class T>
class Package : public AbstractPackage {

private:
    const ModelKey modelKey_;
    const T value_;
public:
    Package(const ModelKey& modelKey, const T& value): modelKey_(modelKey), value_(value) {};
    ModelKey modelKey() const { return modelKey_; };
    const T value() const { return value_; };
    const void* dataptr() const { return &value_; };
};

//Hacky solution to contain array with length
class RawBytePackageValue{
private:
    const size_t len_;
    const uint8_t* data_;
public:
    RawBytePackageValue(uint8_t* data, size_t len) : len_(len), data_(data) {};
    RawBytePackageValue() : len_(0), data_(nullptr) {};
    RawBytePackageValue(const RawBytePackageValue& other) = default;
    const size_t len() const { return len_; }
    const uint8_t* data() const { return data_; }
};

typedef Package<RawBytePackageValue> RawBytePackage;

} }