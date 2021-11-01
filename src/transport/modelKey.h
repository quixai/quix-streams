#pragma once

#include <string>

namespace Quix { namespace Transport {

    /**
     * Class implements abstraction over the model key.
     * Implemented as future preparation for the extension ( e.g. versioning )
     * For now simply wraps the string modelKey.
     */
    class ModelKey{
        private:
            std::string key_;
        public:
            inline ModelKey(const std::string& modelKey = "UNKNOWN"):key_(modelKey){};
            inline ModelKey(const ModelKey& modelKey) = default;
            inline std::string key() const { return key_; };
            inline bool operator<(const ModelKey& other) const {
                return key() < other.key();
            }
            inline bool operator==(const ModelKey& other) const {
                return key() == other.key();
            }
            inline ModelKey& operator=(const ModelKey& other){
                // Guard self assignment
                if (this == &other)
                    return *this;

                key_ = other.key();
                return *this;
            }
    };

} }