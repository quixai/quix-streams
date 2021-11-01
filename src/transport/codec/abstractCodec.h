#pragma once

#include <string>

#include <unordered_map>

namespace Quix {

    class AbstractCodec {
        private:
            const std::string codecKey_;

        protected:
            AbstractCodec(const std::string& codecKey);

        public:
            const std::string & key() const;
    };

}