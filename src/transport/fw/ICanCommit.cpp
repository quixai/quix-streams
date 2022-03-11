#include <vector>

#include "./ICanCommit.h"
#include "../io/transportContext.h"

namespace Quix { namespace Transport {

    using namespace std;

    void ICanCommit::commitSingle(const std::shared_ptr<TransportContext>& transportContext)
    {
        std::vector<std::shared_ptr<TransportContext>> contexts;
        contexts.push_back(transportContext);

        this->commit(contexts);
    };

} }