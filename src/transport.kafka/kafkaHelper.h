#pragma once

#include <mutex>
#include <librdkafka/rdkafkacpp.h>

#include "../transport/io/ISubscriber.h"
#include "../transport/io/package.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/byteArray.h"
#include "./subscriberConfiguration.h"
#include "./topicConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {

class KafkaHelper
{
public:
    static std::shared_ptr<ByteArrayPackage> fromResult( RdKafka::Message *message );
};


} } }