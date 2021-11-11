#pragma once

#include "./IPackage.h"

#include <memory>


namespace Quix { namespace Transport {

/**
 * Interface for providing a class a way to take Package
 */
class IPublisher{

public:
    /**
     * Send a package
     * 
     * @param package The package to send
     */
    virtual void send(std::shared_ptr<IPackage> package) const = 0;
};

} }