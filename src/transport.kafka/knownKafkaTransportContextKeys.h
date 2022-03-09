#pragma once

namespace Quix { namespace Transport { namespace Kafka  {

class KnownKafkaTransportContextKeys {

public:
    /**
     *      The key used to identify the kafka message key
     *      Type is <see cref="string" />
    **/
    static constexpr const char* Key = "KafkaKey";

    /**
     *      The key used to identify the topic the message is from when reading from Kafka
     *      Type is <see cref="string" />
    **/
    static constexpr const char* Topic = "KafkaTopic";

    /**
     *      The key used to identify the partition the message is from when reading from Kafka
     *      Type is <see cref="int" />
    **/
    static constexpr const char* Partition = "KafkaPartition";

    /**
     *      The key used to identify the offset the message has in the source topic and partition
     *      Type is <see cref="long" />
    **/
    static constexpr const char* Offset = "KafkaOffset";

    /**
     *      The key used to identify the time the message was received by kafka
     *      Type is <see cref="DateTime" /> in UTC
    **/
    static constexpr const char* DateTime = "KafkaDataTime";

    /**
     *      The key used to identify the message size read from kafka. This is uncompressed size
     *      Type is <see cref="long" />, measured in bytes
    **/
    static constexpr const char* MessageSize = "KafkaMessageSize";    
};

} } }