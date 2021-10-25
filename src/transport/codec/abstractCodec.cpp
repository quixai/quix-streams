#include <string>

#include "abstractCodec.h"

namespace Quix {
    using namespace std;

    const std::type_info& AbstractCodec::type() const{
        return typeid(this);
    }
}