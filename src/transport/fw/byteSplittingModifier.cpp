#include <string>
#include <memory>
#include <sstream>
#include <functional>

#include "./byteSplittingModifier.h"

namespace Quix { namespace Transport {

    using namespace std;

    ByteSplittingModifier::ByteSplittingModifier(
        IByteSplitter* byteSplitter
    )
    :
    byteSplitter_(byteSplitter)
    {

    }

    void ByteSplittingModifier::send(std::shared_ptr<IPackage> package)
    {
        const auto bytePackage = dynamic_pointer_cast<ByteArrayPackage>(package);



        //TODO: add cancellationToken

        auto it = byteSplitter_->begin(bytePackage);
        auto end = byteSplitter_->end(bytePackage);
        while(it != end){
            onNewPackage(*it);
            ++it;
        }
    };

} }