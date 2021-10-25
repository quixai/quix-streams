#include <memory>
#include <map>
#include <vector>

#include "modelKeyRegistry.h"

namespace Quix {
    using namespace std;

    void ModelKeyRegistry::registerModel(const std::type_index& type, const std::string& modelKey){
        modelKeysToTypes.emplace(modelKey, type);
        typesToModelKeys.emplace(type, modelKey);
    };

    bool ModelKeyRegistry::tryGetType(const std::string& modelKey, std::type_index& type){
        auto it = modelKeysToTypes.find(modelKey);
        if ( it == modelKeysToTypes.end() ){
            return false;
        }
        type = it->second;
        return true;
    };
    bool ModelKeyRegistry::tryGetModelKey(const std::type_index& type, std::string& modelKey){
        auto it = typesToModelKeys.find(type);
        if ( it == typesToModelKeys.end() ){
            return false;
        }
        modelKey = it->second;
        return true;
    };        

}