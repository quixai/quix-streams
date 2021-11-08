#pragma once

#include <string>
#include <functional>

#include "../../io/IPackage.h"
#include "../../codec/abstractCodec.h"
#include "../../codec/codecId.h"
#include "../../io/IPublisher.h"
#include "../../io/ISubscriber.h"

namespace Quix { namespace Transport {

    /**
     * Codec bundle is a composite model with all details necessary for looking up a codec implementation
     */
    class CodecBundle{
        private:
            ModelKey modelKey_;
            CodecId codecId_;
        public:
            CodecBundle(const ModelKey& modelKey, const CodecId& codecId ) : modelKey_(modelKey), codecId_(codecId) {
            };
            const ModelKey& modelKey() const{
                return modelKey_;
            };
            const CodecId& codecId() const{
                return codecId_;
            };
            bool operator==(const CodecBundle& other) const{
                return 
                    modelKey_ == other.modelKey()
                        &&
                    codecId_ == other.codecId()
                    ;
            }
    };

} }