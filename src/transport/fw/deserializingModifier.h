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
     * Modifier, which serializes the package into bytes
     */

    class DeserializingModifier : public IPublisher, ISubscriber{
        private:
            AbstractCodec* getCodec(const std::shared_ptr<TransportPackageValue>& transportPackageValue) const;        
        public:
            std::function<void(std::shared_ptr<IPackage>)> onNewPackage;
            void send(std::shared_ptr<IPackage> package) const;
    };

} }