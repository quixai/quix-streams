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

/**
 * Package<TContent> wrapping the value of type TContent
 *  TContent must be instanceof IModel
 */
template <class TContent, typename std::enable_if<std::is_base_of<IModel, TContent>::value>::type* = nullptr>
class Package : public IPackage {

private:
    const TContent value_; 
    const TransportContext transportContext_;
    const MetaData metaData_;

public:

    /**
     * Initializes a new instance of Package<TContent> with custom ModelKey
     * 
     * @param value content value
     * @param modelKey ModelKey of the value
     * @param metaData The content meta data
     * @param transportContext The extra context relevant for transporting the package
     */
    Package(
        const TContent& value, 
        const ModelKey& modelKey, 
        const MetaData& metaData = MetaData(), 
        const TransportContext& transportContext = TransportContext()
    )
     : 
        value_(value), 
        metaData_(metaData),
        transportContext_(transportContext) 
    {

    };

    /**
     * Initializes a new instance of Package<TContent> with ModelKey from value
     * 
     * @param value content value
     * @param metaData The content meta data
     * @param transportContext The extra context relevant for transporting the package
     */
    Package(
        const TContent& value, 
        const MetaData& metaData = MetaData(), 
        const TransportContext& transportContext = TransportContext()
    )
     : 
        value_(value), 
        metaData_(metaData),
        transportContext_(transportContext)
    {

    };

    /**
     * Initialize Package from the 
     */
    Package(
        std::shared_ptr<IPackage> rawBinary, 
        const TContent& value
    )
     : 
        value_(value), 
        metaData_(rawBinary->metaData())
    {

    };

    /**
     * The content meta data
     */
    const ModelKey modelKey() const 
    { 
        return value_.modelKey();
    };

    /**
     * The transport context of the package
     */
    const TransportContext& transportContext() const
    { 
        return transportContext_;
    };

    /**
     * The content value of the package
     */
    const TContent value() const
    {
        return value_;
    };

    /**
     * The content meta data
     */
    const MetaData metaData() const
    {
        return metaData_;
    };

    /**
     * Check if Package<TContent> is similar to other Package<TContent>
     */
    bool operator==(const Package<TContent>& other) const
    {
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
};

typedef Package<ByteArray> RawBytePackage;

} }