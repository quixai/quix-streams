#include <vector>

#include "./ICanCommit.h"
#include "../io/transportContext.h"

namespace Quix { namespace Transport {

    using namespace std;

    void ICanCommit::commit(const TransportContext& transportContext)
    {
        std::vector<TransportContext> contexts;
        contexts.push_back(transportContext);

        this->commit(contexts);
    };

} }