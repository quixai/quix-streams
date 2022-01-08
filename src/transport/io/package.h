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
    const std::shared_ptr<TransportContext> transportContext_;

public:

    /**
     * Initializes a new instance of Package<TContent> with ModelKey from value
     * 
     * @param value content value
     * @param transportContext The extra context relevant for transporting the package
     */
    Package(
        const TContent& value, 
        const std::shared_ptr<TransportContext>& transportContext = std::shared_ptr<TransportContext>(new TransportContext())
    )
     : 
        value_(value), 
        transportContext_(transportContext)
    {

    };

    /**
     * Initializes a new instance of Package<TContent> with ModelKey from value
     * 
     * @param value content value
     * @param transportContext The extra context relevant for transporting the package
     */
    Package(
        const TContent& value, 
        const TransportContext& transportContext
    )
     : 
        Package(
            value,
            std::shared_ptr<TransportContext>(new TransportContext(transportContext))
        ) 
    {

    };


    /**
     * Create duplicate of the package with modified transport context
     * 
     * @param transportContext updated transportContext
     */
    std::shared_ptr<Package<TContent>> duplicate(const std::shared_ptr<TransportContext>& transportContext)
    {
        return std::shared_ptr<Package<TContent>>( new Package<TContent>( this->value_, transportContext ) );
    }

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
    const std::shared_ptr<TransportContext>& transportContext() const
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


        const bool transportContext1Null = transportContext_.get() == nullptr;
        const bool transportContext2Null = other.transportContext().get() == nullptr;
        const bool isTransportContextSame = 
                                (transportContext1Null && transportContext2Null)
                                ||
                                (
                                    !transportContext1Null && 
                                    !transportContext2Null && 
                                    *transportContext_ == *(other.transportContext())
                                )
                                ;

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