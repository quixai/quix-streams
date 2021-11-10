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
            /**
             * Initializes a new instance of
             *  
             * @param modelKey The key to identify the model
             * @param codecId The id of the codec used for model serialization
             */
            CodecBundle(const ModelKey& modelKey, const CodecId& codecId )
             : 
             modelKey_(modelKey), 
             codecId_(codecId) 
            {
                
            };
            /**
             * Gets the key to identify the Data
             */
            const ModelKey& modelKey() const
            {
                return modelKey_;
            };
            /**
             * Gets the codec id to used to serialize the model
             */
            const CodecId& codecId() const
            {
                return codecId_;
            };
            bool operator==(const CodecBundle& other) const
            {
                return 
                    modelKey_ == other.modelKey()
                        &&
                    codecId_ == other.codecId()
                    ;
            }
    };

} }