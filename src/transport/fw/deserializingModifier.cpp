#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include "./deserializingModifier.h"
#include "./exceptions/deserializingException.h"
#include "./helpers/transportPackageValueCodec.h"
#include "../codec/codecId.h"
#include "../registry/modelKeyRegistry.h"
#include "../io/IPackage.h"
#include "../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    void DeserializingModifier::send(std::shared_ptr<IPackage> package) const
    {
        //TODO: add cancellationToken

        const auto& transportMessageValue = TransportPackageValueCodec::deserialize((static_cast<ByteArrayPackage*>(&*package))->value());
        const auto codecBundle = transportMessageValue->codecBundle();

        auto codec = getCodec(transportMessageValue);
        const auto deserializedPackage = codec->deserialize(transportMessageValue->value());


        onNewPackage(deserializedPackage); 
    };


    AbstractCodec* DeserializingModifier::getCodec(const std::shared_ptr<TransportPackageValue>& transportPackageValue) const
    {
        //TODO: get modelKey
        const auto& codecBundle = transportPackageValue->codecBundle();
        const auto codec = CodecRegistry::instance()->retrieveCodec(codecBundle.modelKey(), codecBundle.codecId());

        if(codec == nullptr){
            std::stringstream ss;
            ss << "Failed to deserialize '" << codecBundle.modelKey().key() << "' because there is no codec registered for it.";
            throw DeserializingException(ss.str());
        }

        return codec;
    }

} }