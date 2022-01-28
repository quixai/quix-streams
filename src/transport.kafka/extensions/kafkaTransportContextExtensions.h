#pragma once

#include <mutex>
#include <string>
#include <chrono>
#include <functional>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "../../utils/timer.h"

#include "../../transport/io/IPublisher.h"
#include "../../transport/io/IPackage.h"
#include "../../transport/io/byteArray.h"
#include "../IKafkaPublisher.h"
#include "../topicConfiguration.h"
#include "../knownKafkaTransportContextKeys.h"

namespace Quix { namespace Transport { namespace Kafka  {

class KafkaTransportContextExtensions {

    TransportContext* const context_;

public:

    KafkaTransportContextExtensions(TransportContext* const context)
    :
    context_(context)
    {

    }

    bool tryGetKafkaCommitDetails(std::string& topic, int& partition, int64_t& offset) const
    {
        if( !context_->tryGetValue( KnownKafkaTransportContextKeys::Topic, topic ) )
        {
            return false;
        }
        std::string offsetStr;
        if( !context_->tryGetValue( KnownKafkaTransportContextKeys::Offset, offsetStr ) )
        {
            return false;
        }
        offset = atoll( offsetStr.c_str() );

        std::string partitionStr;
        if( !context_->tryGetValue( KnownKafkaTransportContextKeys::Partition, partitionStr ) )
        {
            return false;
        }
        partition = atoll( partitionStr.c_str() );

        return true;


    }
};


} } }