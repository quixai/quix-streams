#include <string>

#include "./abstractCodec.h"
#include "./codecId.h"

namespace Quix { namespace Transport {
    using namespace std;

    AbstractCodec::AbstractCodec(const CodecId& codecId) : codecId_(codecId) {

    }

    const CodecId& AbstractCodec::codecId() const {
        return codecId_;
    }

} }