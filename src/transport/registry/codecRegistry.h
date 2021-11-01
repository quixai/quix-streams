#pragma once

#include <memory>
#include <map>
#include <vector>

#include "../codec/abstractCodec.h"
#include "../modelKey.h"

namespace Quix::Transport {

    class CodecRegistry{

        private:
            CodecRegistry(){};

            /**
             *  Map containing all registered codecs stored in vector with modelKey as key. 
             * 
             *  The last element from vector is used for serializing ( default )
             *   rest is eventually used for deserialization.
             */
            std::map<ModelKey, std::vector<AbstractCodec*>> codecs;

        public:

            // BEGIN // Make class singleton
            CodecRegistry(CodecRegistry const&) = delete;
            CodecRegistry& operator=(CodecRegistry const&) = delete;
            static std::shared_ptr<CodecRegistry> instance()
            {
                static std::shared_ptr<CodecRegistry> s{new CodecRegistry};
                return s;
            }
            // END // Make class singleton


            /**
             * @brief Registers a codec for the provided key.
             * 
             * If multiple codecs are registered for the same ModelKey, the last one registered will be used for writing
             * 
             * @param modelKey model key to be registered
             * @params codec codec
             */
            void registerCodec(const ModelKey& modelKey, AbstractCodec*);

            /**
             * @brief Retrieves registered codecs for the key
             * 
             * @param modelKey The model key to retrieve codecs for
             * @returns Previously registered <see cref="ICodec"/> or an empty collection if none found
             */
            std::vector<AbstractCodec*>& retrieveCodecs(const ModelKey& modelKey);

            /**
             * @brief Retrieves registered codecs for the model key and codec id
             * 
             * @param modelKey The model key to retrieve codecs for
             * @param codecKey String representation of the codec
             * 
             * @returns nullptr if no codec found, otherwise matching codec implementation
             */
            AbstractCodec* retrieveCodec(const ModelKey& modelKey, const std::string& codecKey);

            /**
             * @brief Clear all codecs registered for model
             * 
             * @param modelKey The model key to clear codecs for
             */
            void clearCodecs(const ModelKey& modelKey);

        
    };


}