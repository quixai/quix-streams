#include <string>

#include "./ICodec.h"
#include "./codecId.h"

namespace Quix { namespace Transport {
    using namespace std;

    BaseCodec::BaseCodec(const CodecId& codecId)
     : 
     codecId_(codecId) 
    {

    }

    const CodecId& BaseCodec::codecId() const 
    {
        return codecId_;
    }

} }