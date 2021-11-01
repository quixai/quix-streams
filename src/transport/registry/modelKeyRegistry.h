#pragma once

#include <memory>
#include <map>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <map>
#include <vector>

#include "../codec/abstractCodec.h"
#include "../modelKey.h"

namespace Quix { namespace Transport {

    /**
     * Contains bi-directional mapping between codecKey and modelKey.
     * Supports the fast O(1) lookup according to both keys.
     */
    class ModelKeyRegistry{

        private:
            ModelKeyRegistry(){};

            //bi-directional map stored as two maps
            std::map<ModelKey, std::string> modelKeysToCodecKeys;
            std::map<std::string, ModelKey> typesToModelKeys;

        public:

            // BEGIN // Make class singleton
            ModelKeyRegistry(ModelKeyRegistry const&) = delete;
            ModelKeyRegistry& operator=(ModelKeyRegistry const&) = delete;
            static std::shared_ptr<ModelKeyRegistry> instance()
            {
                static std::shared_ptr<ModelKeyRegistry> s{new ModelKeyRegistry};
                return s;
            }
            // END // Make class singleton
            
            /**
             * @brief Register model 
             * 
             * Add item into the bi-directional map
             * 
             * @param modelKey String representation of the model  key
             * @param codecKey String representation of the codec key
             */
            void registerModel(const std::string& codecKey, const ModelKey& modelKey);

            /**
             * @brief Get last codecKey for specified modelKey
             * 
             * Search in the bi-directional map for specified modelKey
             * 
             * @param modelKey String representation of the model key to be searched
             * @param codecKey output parameter where is the codecKey returned
             * 
             * @returns true if the modelKey has been found in map, false otherwise
             */
            bool tryGetCodecKey(const ModelKey& modelKey, std::string& codecKey);

            /**
             * @brief Get last modelKey for specified codecKey
             * 
             * Search in the bi-directional map for specified codecKey
             * 
             * @param codecKey String representation of the codec key to be searched
             * @param modelKey output parameter where is the modelKey returned
             * 
             * @returns true if the codecKey has been found in map, false otherwise
             */
            bool tryGetModelKey(const std::string& codecKey, ModelKey& modelKey);        

    };

    //for fancy printing using "cout <<"
    inline std::ostream& operator<< (std::ostream &out, ModelKey const& data) {
        out << "ModelKey(" << data.key() << ')';
        return out;
    }


} }