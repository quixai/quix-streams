#include "codecRegistry.h"
#include "modelKeyRegistry.h"

#include <algorithm>
#include <iostream>

namespace Quix {
    using namespace std;

    /**
     * Registers a codec for the provided key. If multiple codecs are registered for the same ModelKey, the last one registered will be used for writing
     * @param model modelName to be registered
     * @param codec codec
     */
    void CodecRegistry::registerCodec(const string& model, AbstractCodec* codec){

        //not found model >> creating a element of key
        if ( codecs.find(model) == codecs.end() ) {
            codecs[model] = {};
        }

        //find if the codec with same codecId exists
        //TODO: this is slow due to RTTI dispatch of virtual id() >> redo to cache the ids
        auto& modelCodecs = codecs[model];

        auto codecId = codec->id();
        auto it = std::find_if(modelCodecs.begin(), modelCodecs.end(), [codecId](AbstractCodec* val){return val->id() == codecId;});
        if( it == modelCodecs.end() ) {
            //no codec found
            modelCodecs.push_back(codec);
        }else{
            //replaces codec
            *it = codec;
        }

        static auto modelKeyRegistry = ModelKeyRegistry::instance();
        modelKeyRegistry->registerModel(codec->type(), model);
    }

    /**
     * Retrieves registered codecs for the key
     * @param modelKey The model key to retrieve codecs for
     * @return Previously registered <see cref="ICodec"/> or an empty collection if none found
     */
    vector<AbstractCodec*>& CodecRegistry::retrieveCodecs(const string& modelKey){
        if ( codecs.find(modelKey) == codecs.end() ){
            static vector<AbstractCodec*> ret = {};
            return ret;
        }
        return codecs[modelKey];
    }

        /**
     * Retrieves registered codecs for the model key and codec id
     * @param modelKey The model key to retrieve codecs for
     * @return Previously registered <see cref="ICodec"/> or an empty collection if none found
     */
    AbstractCodec* CodecRegistry::retrieveCodec(const string& modelKey, const string& codecId){
        auto modelCodecs = retrieveCodecs(modelKey);
        //TODO: this is slow due to RTTI dispatch of virtual id() >> redo to cache the ids
        auto el = std::find_if(modelCodecs.begin(), modelCodecs.end(), [codecId](AbstractCodec* val){
            return val->id() == codecId;
        });
        return el == modelCodecs.end()
                    ?
                nullptr
                    :
                *el;
    }

    /**
     * Clear codecs registered for model
     * @param modelKey The model key to clear codecs for
     */
    void CodecRegistry::clearCodecs(const string& modelKey){
        auto it = codecs.find(modelKey);
        if ( it != codecs.end() ){
            codecs.erase(it);
        }
    }


}

