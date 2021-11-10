#include <string>
#include <sstream>
#include <memory>
#include <functional>

#include "./serializingModifier.h"
#include "./exceptions/serializingException.h"
#include "./helpers/transportPackageValue.h"
#include "./helpers/transportPackageValueCodec.h"
#include "../registry/modelKeyRegistry.h"
#include "../io/IPackage.h"
#include "../registry/codecRegistry.h"

namespace Quix { namespace Transport {

    void SerializingModifier::send(std::shared_ptr<IPackage> package) const
    {
        //TODO: add cancellationToken
        const auto& modelKey = package->modelKey();

        auto codec = CodecRegistry::instance()->retrieveFirstCodec(modelKey);
        if(codec == nullptr){
            std::stringstream ss;
            ss << "Failed to serialize '" << modelKey.key() << "' because there is no codec registered for it.";
            throw SerializingException(ss.str());
        }

        auto bytePackage = serializePackage(package, codec, CodecBundle(modelKey, codec->codecId()));
        onNewPackage(bytePackage); 
    };

    std::shared_ptr<RawBytePackage> SerializingModifier::serializePackage(std::shared_ptr<IPackage> package, AbstractCodec* codec, const CodecBundle& codecBundle) const{
        ByteArray serializedData = codec->serialize(package);
        if(serializedData.begin() == nullptr){
            const ModelKey& modelKey = codecBundle.modelKey();
            std::stringstream ss;
            ss << "Failed to serialize '" << modelKey.key() << "' because codec returned nullptr.";
            throw SerializingException(ss.str());
        }

        const auto& metadata = package->metaData();
        auto wrappedInPackage = TransportPackageValueCodec::serialize(
            std::shared_ptr<TransportPackageValue>(
                new TransportPackageValue(
                    std::shared_ptr<RawBytePackage>(
                        new RawBytePackage(serializedData, metadata)
                    ),
                    codecBundle
                )
            )
        );

        return std::shared_ptr<RawBytePackage>(
            new RawBytePackage(wrappedInPackage, metadata)
        );
    }

    

} }