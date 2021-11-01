#include "codecRegistry.h"
#include "modelKeyRegistry.h"

#include <algorithm>
#include <iostream>

namespace Quix { namespace Transport {
    using namespace std;

    void CodecRegistry::registerCodec(const ModelKey& model, AbstractCodec* codec){

        //not found model >> creating a element of key
        if ( codecs.find(model) == codecs.end() ) {
            codecs[model] = {};
        }

        //find if the codec with same codecId exists
        //TODO: this is slow due to RTTI dispatch of virtual id() >> redo to cache the ids
        auto& modelCodecs = codecs[model];

        auto codecKey = codec->key();
        auto it = std::find_if(modelCodecs.begin(), modelCodecs.end(), [codecKey](AbstractCodec* val){
            return val->key() == codecKey;
        });
        if( it == modelCodecs.end() ) {
            //no codec found
            modelCodecs.push_back(codec);
        }else{
            //replaces codec
            *it = codec;
        }

        static auto modelKeyRegistry = ModelKeyRegistry::instance();
        modelKeyRegistry->registerModel(codecKey, model);
    }

    vector<AbstractCodec*>& CodecRegistry::retrieveCodecs(const ModelKey& modelKey){
        if ( codecs.find(modelKey) == codecs.end() ){
            static vector<AbstractCodec*> ret = {};
            return ret;
        }
        return codecs[modelKey];
    }


    AbstractCodec* CodecRegistry::retrieveCodec(const ModelKey& modelKey, const string& codecKey){
        auto modelCodecs = retrieveCodecs(modelKey);

        //find first codec from the list which has the key matching codecKey
        auto el = std::find_if(modelCodecs.begin(), modelCodecs.end(), [codecKey](AbstractCodec* val){
            return val->key() == codecKey;
        });

        return el == modelCodecs.end()
                    ?
                nullptr
                    :
                *el;
    }

    void CodecRegistry::clearCodecs(const ModelKey& modelKey){
        auto it = codecs.find(modelKey);
        if ( it != codecs.end() ){
            codecs.erase(it);
        }
    }

} }

