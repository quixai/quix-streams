#pragma once

#include <type_traits>
#include <memory>
#include <vector>

#include "./transportContext.h"
#include "./IModel.h"
#include "./IPackage.h"
#include "./byteArray.h"

namespace Quix { namespace Transport {

/**
 * Package<TContent> wrapping the value of type TContent
 *  TContent must be instanceof IModel
 */

// std::enable_if is saying that the TContent extends IModel
template <class TContent, typename std::enable_if<std::is_base_of<IModel, TContent>::value>::type* = nullptr>
class Package : public IPackage {

private:
    const TContent value_; 
    const TransportContext transportContext_;

public:

    /**
     * Initializes a new instance of Package<TContent> with ModelKey from value
     * 
     * @param value content value
     * @param transportContext The extra context relevant for transporting the package
     */
    Package(
        const TContent& value, 
        const TransportContext& transportContext = TransportContext()
    )
     : 
        value_(value), 
        transportContext_(transportContext)
    {

    };

    /**
     * Initialize meta information Package from previous parent
     * but override the value with new one
     * 
     * @param parent package to take information from
     * @param value new value of the package
     */
    Package(
        std::shared_ptr<IPackage> parent, 
        const TContent& value
    )
     : 
        value_(value), 
        transportContext_(parent->transportContext())
    {

    };

    /**
     * The unique representation of the model as ModelKey
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
     * Check if Package<TContent> is similar to other Package<TContent>
     */
    bool operator==(const Package<TContent>& other) const
    {
        const bool isValueSame = value_ == other.value();
        const bool isModelKeySame = value_.modelKey() == other.modelKey();
        const bool isTransportContextSame = transportContext_ == other.transportContext();

        return 
            isValueSame
                &&
            isModelKeySame
                &&
            isTransportContextSame 
            ;
    };
};

typedef Package<ByteArray> ByteArrayPackage;

} }