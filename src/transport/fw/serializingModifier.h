#pragma once

#include <string>
#include <memory>
#include <functional>

#include "../io/package.h"
#include "../io/IPackage.h"
#include "../codec/abstractCodec.h"
#include "./codecs/codecBundle.h"
#include "./IModifier.h"

#include "../../utils/eventHandler.h"

namespace Quix { namespace Transport {

/**
 * Modifier, which serializes the package into bytes
 */
class SerializingModifier : public IModifier{

private:
    /**
     * Serialize Model package into byte package
     * 
     * @param package The package to serialize
     * @param codec The codec to use to serialize the package
     * @param codecBundle The model details to put inside the package value
     */
    std::shared_ptr<ByteArrayPackage> serializePackage(
        std::shared_ptr<IPackage> package, 
        AbstractCodec* codec, 
        const CodecBundle& codecBundle
    ) const;

public:

    /**
     * Send a package, which the modifier attemptes to serialize. Serialization result is raised via <see cref="OnNewPackage"/>
     *
     * @param package The package to serialize
     * 
     * @throws SerializationException are raised for unsuccessful serialization when package's lazy value is evaluated.
     */
    void send(std::shared_ptr<IPackage> package);
};

} }