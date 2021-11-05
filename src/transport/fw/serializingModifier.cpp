#include <string>
#include <sstream>
#include <functional>

#include "./serializingModifier.h"
#include "./exceptions/serializingException.h"
#include "./helpers/transportPackageValueCodec.h"
#include "../registry/modelKeyRegistry.h"
#include "../io/IPackage.h"
#include "../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    void SerializingModifier::send(IPackage* package)
    {
        //TODO: add cancellationToken
        // ModelKey modelKey;
        // ModelKeyRegistry::instance()->tryGetModelKey(package->modelKey(), modelKey);
        auto modelKey = package->modelKey();
        // if (modelKey == ModelKey.WellKnownModelKeys.Default)
        // {
        //     modelKey = new ModelKey(package.Type);
        // }

        auto codec = CodecRegistry::instance()->retrieveFirstCodec(modelKey);
        if(codec == nullptr){
            std::stringstream ss;
            ss << "Failed to serialize '" << modelKey.key() << "' because there is no codec registered for it.";
            throw SerializingException(ss.str());
        }

        auto bytePackage = serializePackage(package, codec, modelKey);
        onNewPackage(bytePackage); 
    };

    RawBytePackage* SerializingModifier::serializePackage(IPackage* package, AbstractCodec* codec, const ModelKey& modelKey) const{
        try{
            RawBytePackageValue serializedData = codec->serialize(package->dataptr());
            delete package;
            if(serializedData.begin() == nullptr){
                std::stringstream ss;
                ss << "Failed to serialize '" << modelKey.key() << "' because codec returned nullptr.";
                throw SerializingException(ss.str());
            }

            const auto& metadata = package->metaData();
            auto wrappedInPackage = TransportPackageValueCodec::Serialize(new RawBytePackage(modelKey, serializedData, metadata));
            return new RawBytePackage(modelKey, wrappedInPackage, metadata);
        }catch(...){
            delete package;
            throw;
        }
    }

    

} }