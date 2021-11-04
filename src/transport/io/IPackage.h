#pragma once

#include <memory>
#include <vector>

#include "../fw/modelKey.h"
#include "../io/metaData.h"
#include "./rawBytePackageValue.h"

namespace Quix { namespace Transport {

class IPackage{
public:
    virtual ModelKey modelKey() const = 0;
    virtual const void* dataptr() const = 0;
    virtual const MetaData metaData() const = 0;
};

template <class T>
class Package : public IPackage {

private:
    const ModelKey modelKey_;
    const T value_;
    const MetaData metaData_;
public:
    Package(const ModelKey& modelKey, const T& value, const MetaData& metaData) : 
        modelKey_(modelKey), 
        value_(value), 
        metaData_(metaData) 
    {};
    ModelKey modelKey() const { return modelKey_; };
    const T value() const { return value_; };
    bool operator==(const Package<T>& other) const {
        bool is1 = value_ == other.value();
        bool is2 = modelKey_ == other.modelKey();
        bool is3 = metaData_ == other.metaData();

        return 
            value_ == other.value()
                &&
            modelKey_ == other.modelKey() 
                &&
            metaData_ == other.metaData() 
            ;
    };
    const MetaData metaData() const { return metaData_; };
    const void* dataptr() const { return &value_; };
};

typedef Package<RawBytePackageValue> RawBytePackage;

} }