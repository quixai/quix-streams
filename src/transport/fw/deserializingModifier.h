#pragma once

#include <string>
#include <functional>
#include <memory>

#include "./helpers/transportPackageValue.h"
#include "../io/IPackage.h"
#include "../codec/abstractCodec.h"
#include "../codec/codecId.h"
#include "../io/IPublisher.h"
#include "../io/ISubscriber.h"

namespace Quix { namespace Transport {

/**
 * Modifier, which deserializes the package into the model described in the package.
 */
class DeserializingModifier : public IPublisher, ISubscriber{

private:
    /**
     * Retrieves the codec from the provided package value
     * 
     * @param transportPackageValue package to deserialize
     * 
     * @return Pointer to the codec from the package value
     */
    AbstractCodec* getCodec(const std::shared_ptr<TransportPackageValue>& transportPackageValue) const;        

public:

    /**
     * The callback that is used when deserialized package is available
     */
    std::function<void(std::shared_ptr<IPackage>)> onNewPackage;

    /**
     * Send a package, which the modifier attemptes to deserialize. Deserialization results is raised via onNewPackage
     * 
     * @param package The package to deserialize
     * 
     * @throw When deserialization fails due to unknown codec or invalid data for codec
     */
    void send(std::shared_ptr<IPackage> package) const;

};

} }