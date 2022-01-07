#pragma once

#include <string>

namespace Quix { namespace Transport {

/**
 * The key used to uniquely identify a model
 * 
 * Class implements abstraction over the model key.
 * Implemented as future preparation for the extension ( e.g. versioning )
 * For now simply wraps the string modelKey.
 */
class ModelKey{

private:
    std::string key_;

public:
    /**
     * Creates a new ModelKey using the provided string key
     * 
     * @param modelKey the string key
     */
    inline ModelKey(const std::string& modelKey = "UNKNOWN")
     : 
     key_(modelKey)
    {

    };

    /**
     * Copy constructor
     */
    inline ModelKey(const ModelKey& modelKey) = default;

    inline std::string key() const { return key_; };

    /**
     * Define ordering of the ModelKey class
     * 
     *  used mainly to satisfy std containers ( especially std::map )
     */
    inline bool operator<(const ModelKey& other) const
    {
        return key() < other.key();
    }

    /**
     * Define equality to another ModelKey class
     */
    inline bool operator==(const ModelKey& other) const
    {
        return key() == other.key();
    }

    /**
     * Copy operator
     */
    inline ModelKey& operator=(const ModelKey& other){
        // Guard self assignment
        if (this == &other)
            return *this;

        key_ = other.key();
        return *this;
    }

    /**
     * @brief get default model key associated with the class name
     * 
     * @tparam T 
     * @return ModelKey 
     */
    template<class T>
    ModelKey static forType( )
    {
        return ModelKey( typeid(T).name() );
    }

};

} }