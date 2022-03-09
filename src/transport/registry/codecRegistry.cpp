#include "codecRegistry.h"
#include "modelKeyRegistry.h"

#include "../fw/codecs/byteCodec.h"

#include <algorithm>
#include <iostream>

namespace Quix { namespace Transport {
    using namespace std;

    CodecRegistry::CodecRegistry()
    {
        initializeCodecs();
    }

    void CodecRegistry::initializeCodecs()
    {
        //initialize bytecodec
        static ByteCodec byteCodec;
        this->registerCodec(
            ByteArray().modelKey(), 
            &byteCodec
        );
   }


    void CodecRegistry::registerCodec(const ModelKey& model, const BaseCodec* codec)
    {

        //not found model >> creating a element of key
        if ( codecs.find(model) == codecs.end() ) 
        {
            codecs[model] = {};
        }

        //find if the codec with same codecId exists
        auto& modelCodecs = codecs[model];

        auto codecKey = codec->codecId();
        auto it = std::find_if(modelCodecs.begin(), modelCodecs.end(), [&](const BaseCodec* val){
            return val->codecId() == codecKey;
        });
        if( it == modelCodecs.end() ) 
        {
            //no codec found
            modelCodecs.push_back(codec);
        }
        else
        {
            //replaces codec
            *it = codec;
        }

        static auto modelKeyRegistry = ModelKeyRegistry::instance();
        modelKeyRegistry->registerModel(codecKey, model);
    }

    vector<const BaseCodec*>& CodecRegistry::retrieveCodecs(const ModelKey& modelKey)
    {
        if ( codecs.find(modelKey) == codecs.end() )
        {
            static vector<const BaseCodec*> ret = {};
            return ret;
        }
        return codecs[modelKey];
    }


    const BaseCodec* CodecRegistry::retrieveCodec(const ModelKey& modelKey, const string& codecKey)
    {
        auto modelCodecs = retrieveCodecs(modelKey);

        //find first codec from the list which has the key matching codecKey
        auto el = std::find_if(modelCodecs.begin(), modelCodecs.end(), [&](const BaseCodec* val){
            return val->codecId() == codecKey;
        });

        return el == modelCodecs.end()
                    ?
                nullptr
                    :
                *el;
    }

    const BaseCodec* CodecRegistry::retrieveFirstCodec(const ModelKey& modelKey)
    {
        auto modelCodecs = retrieveCodecs(modelKey);

        auto it = modelCodecs.begin();        
        //find first codec from the list which has the key matching codecKey
        return it == modelCodecs.end()
                    ?
                nullptr
                    :
                *it;
    }


    void CodecRegistry::clearCodecs(const ModelKey& modelKey)
    {
        auto it = codecs.find(modelKey);
        if ( it != codecs.end() )
        {
            codecs.erase(it);
        }
    }

} }

