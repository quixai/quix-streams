#include <string>

#include "abstractCodec.h"

namespace Quix { namespace Transport {
    using namespace std;

    AbstractCodec::AbstractCodec(const std::string& codecKey) : codecKey_(codecKey) {

    }

    const std::string& AbstractCodec::key() const {
        return codecKey_;
    }

} }