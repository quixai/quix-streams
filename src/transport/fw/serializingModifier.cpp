#include <string>
#include <functional>

#include "./serializingModifier.h"
#include "./exceptions/serializingException.h"
#include "../registry/modelKeyRegistry.h"
#include "../io/abstractPackage.h"
#include "../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    void SerializingModifier::send(AbstractPackage* package)
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
             throw SerializingException(std::string("Failed to serialize '")+modelKey.key()+"' because there is no codec registered for it.");
        }

        auto bytePackage = serializePackage(package, codec, modelKey);
        onNewPackage(bytePackage); 
    };

    RawBytePackage* SerializingModifier::serializePackage(AbstractPackage* package, AbstractCodec* codec, const ModelKey& modelKey) const{
        try{
            RawBytePackageValue serializedData = codec->serialize(package->dataptr());
            delete package;
            if(serializedData.data() == nullptr){
                throw SerializingException(std::string("Failed to serialize '")+modelKey.key()+"' because codec returned nullptr.");
            }
            return new RawBytePackage(modelKey, serializedData);
        }catch(...){
            delete package;
            throw;
        }
    }

    

} }