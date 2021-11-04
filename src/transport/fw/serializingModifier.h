#pragma once

#include <string>
#include <functional>

#include "../io/IPackage.h"
#include "../codec/abstractCodec.h"
#include "../io/IPublisher.h"

namespace Quix { namespace Transport {

    /**
     * Modifier, which serializes the package into bytes
     */

    class SerializingModifier : public IPublisher{
        private:
            RawBytePackage* serializePackage(IPackage* package, AbstractCodec* codec, const ModelKey& modelKey) const;

        public:
            std::function<void(RawBytePackage*)> onNewPackage;
            void send(IPackage* package);
    };

} }