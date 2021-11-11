#include <memory>
#include <map>
#include <vector>

#include "modelKeyRegistry.h"

namespace Quix { namespace Transport {
    using namespace std;

    void ModelKeyRegistry::registerModel(const std::string& codecKey, const ModelKey& modelKey)
    {
        codecKeysToModelKeys.emplace(codecKey, modelKey);
    };

    bool ModelKeyRegistry::tryGetModelKey(const std::string& codecKey, ModelKey& modelKey)
    {
        auto it = codecKeysToModelKeys.find(codecKey);
        if ( it == codecKeysToModelKeys.end() )
        {
            return false;
        }
        modelKey = it->second;
        return true;
    };        

} }