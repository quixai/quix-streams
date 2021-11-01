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
            std::unordered_map<std::string, std::string> modelKeysToCodecKeys;
            std::unordered_map<std::string, std::string> typesToModelKeys;

        public:
            ModelKeyRegistry(ModelKeyRegistry const&) = delete;
            ModelKeyRegistry& operator=(ModelKeyRegistry const&) = delete;
            static std::shared_ptr<ModelKeyRegistry> instance()
            {
                static std::shared_ptr<ModelKeyRegistry> s{new ModelKeyRegistry};
                return s;
            }
            
            void registerModel(const std::string& type, const std::string& modelKey);
            bool tryGetCodecKey(const std::string& modelKey, std::string& type);
            bool tryGetModelKey(const std::string& type, std::string& modelKey);        
    };


}