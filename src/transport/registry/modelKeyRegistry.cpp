#include <memory>
#include <map>
#include <vector>

#include "modelKeyRegistry.h"

namespace Quix {
    using namespace std;

    void ModelKeyRegistry::registerModel(const std::string& codecKey, const std::string& modelKey){
        modelKeysToCodecKeys.emplace(modelKey, codecKey);
        typesToModelKeys.emplace(codecKey, modelKey);
    };

    bool ModelKeyRegistry::tryGetCodecKey(const std::string& modelKey, std::string& codecKey){
        auto it = modelKeysToCodecKeys.find(modelKey);
        if ( it == modelKeysToCodecKeys.end() ){
            return false;
        }
        codecKey = it->second;
        return true;
    };
    bool ModelKeyRegistry::tryGetModelKey(const std::string& codecKey, std::string& modelKey){
        auto it = typesToModelKeys.find(codecKey);
        if ( it == typesToModelKeys.end() ){
            return false;
        }
        modelKey = it->second;
        return true;
    };        

}