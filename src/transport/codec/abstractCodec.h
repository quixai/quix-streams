#pragma once

#include "../io/IPackage.h"

#include <string>
#include <memory>
#include <memory>
#include <vector>
#include <unordered_map>


#include "./codecId.h"
#include "../io/IPackage.h"


namespace Quix { namespace Transport {

class AbstractCodec {
    private:
        const CodecId codecId_;

    protected:
        AbstractCodec(const CodecId& codecId);

    public:
        const CodecId& codecId() const;
        virtual RawBytePackageValue serialize(const std::shared_ptr<IPackage> obj) const = 0;
        virtual const std::shared_ptr<IPackage> deserialize(const RawBytePackageValue& value) const = 0;

};

} }