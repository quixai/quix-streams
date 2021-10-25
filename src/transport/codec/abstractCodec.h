#pragma once

#include <string>

namespace Quix {

    class AbstractCodec{
        public:
            virtual std::string id() = 0;
            const std::type_info& type() const;
            // virtual char* serialize(void* obj) = 0;
    };

}