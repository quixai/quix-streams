#pragma once

#include <string>
#include <functional>

#include "../io/abstractPackage.h"
#include "../codec/abstractCodec.h"
#include "../io/abstractInput.h"

namespace Quix { namespace Transport {

    /**
     * Modifier, which serializes the package into bytes
     */

    class SerializingModifier : public AbstractInput{
        private:
            RawBytePackage* serializePackage(AbstractPackage* package, AbstractCodec* codec, const ModelKey& modelKey) const;

        public:
            std::function<void(RawBytePackage*)> onNewPackage;
            void send(AbstractPackage* package);
    };

} }