#pragma once

#include <string>
#include <memory>
#include <functional>

#include "../io/Package.h"
#include "../io/IPackage.h"
#include "../codec/abstractCodec.h"
#include "./codecs/codecBundle.h"
#include "../io/IPublisher.h"

namespace Quix { namespace Transport {

    /**
     * Modifier, which serializes the package into bytes
     */

    class SerializingModifier : public IPublisher{
        private:
            std::shared_ptr<RawBytePackage> serializePackage(std::shared_ptr<IPackage> package, AbstractCodec* codec, const CodecBundle& codecBundle) const;

        public:
            std::function<void(std::shared_ptr<RawBytePackage>)> onNewPackage;
            void send(std::shared_ptr<IPackage> package) const;
    };

} }