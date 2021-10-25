#pragma once

#include <memory>
#include <map>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

#include "../codec/abstractCodec.h"

namespace Quix {

    class ModelKeyRegistry{

        private:
            ModelKeyRegistry(){};
            std::unordered_map<std::string, std::type_index> modelKeysToTypes;
            std::unordered_map<std::type_index, std::string> typesToModelKeys;

        public:
            ModelKeyRegistry(ModelKeyRegistry const&) = delete;
            ModelKeyRegistry& operator=(ModelKeyRegistry const&) = delete;
            static std::shared_ptr<ModelKeyRegistry> instance()
            {
                static std::shared_ptr<ModelKeyRegistry> s{new ModelKeyRegistry};
                return s;
            }
            
            void registerModel(const std::type_index& type, const std::string& modelKey);
            bool tryGetType(const std::string& modelKey, std::type_index& type);
            bool tryGetModelKey(const std::type_index& type, std::string& modelKey);        
    };


}