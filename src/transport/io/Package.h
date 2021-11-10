#pragma once

#include <type_traits>
#include <memory>
#include <vector>

#include "./transportContext.h"
#include "./IModel.h"
#include "./IPackage.h"
#include "../io/metaData.h"
#include "./byteArray.h"

namespace Quix { namespace Transport {

template <class T, typename std::enable_if<std::is_base_of<IModel, T>::value>::type* = nullptr>
class Package : public IPackage {

private:
    const T value_; 
    const TransportContext transportContext_;
    const MetaData metaData_;
public:
    Package(const T& value, const ModelKey& modelKey, const MetaData& metaData = MetaData(), const TransportContext& transportContext = TransportContext()) : 
        value_(value), 
        metaData_(metaData),
        transportContext_(transportContext) 
    {};
    Package(const T& value, const MetaData& metaData = MetaData(), const TransportContext& transportContext = TransportContext()) : 
        value_(value), 
        metaData_(metaData),
        transportContext_(transportContext)
    {};
    Package(std::shared_ptr<IPackage> rawBinary, const T& value) : 
        value_(value), 
        metaData_(rawBinary->metaData()) 
    {};
    const ModelKey modelKey() const { return value_.modelKey(); };
    const TransportContext& transportContext() const { return transportContext_; };
    const T value() const { return value_; };
    bool operator==(const Package<T>& other) const {
        const bool isValueSame = value_ == other.value();
        const bool isModelKeySame = value_.modelKey() == other.modelKey();
        const bool isMetaDataSame = metaData_ == other.metaData();

        return 
            isValueSame
                &&
            isModelKeySame
                &&
            isMetaDataSame 
            ;
    };
    const MetaData metaData() const { return metaData_; };
    const void* dataptr() const { return &value_; };
};

typedef Package<ByteArray> RawBytePackage;

} }