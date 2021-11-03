#pragma once

#include "../io/abstractPackage.h"

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Quix { namespace Transport {

    class AbstractCodec {
        private:
            const std::string codecKey_;

        protected:
            AbstractCodec(const std::string& codecKey);

        public:
            const std::string & key() const;
            RawBytePackageValue serialize(const void* obj) const{
                //TODO: do clever allocation
                return RawBytePackageValue();
            };
    };

} }