#pragma once

#include <mutex>
#include <rdkafkacpp.h>

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
    /**
     *  Parses the Kafka Message into a PackageByteArray ( alias for Package<ByteArray> )
     * 
     * @param consumeResult The consume result to parse
     * 
     * @returns The package
     **/
    static std::shared_ptr<ByteArrayPackage> fromResult( RdKafka::Message *message );
};


} } }