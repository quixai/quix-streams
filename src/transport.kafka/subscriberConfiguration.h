#pragma once

#include <string>
#include <sstream>
#include <map>
#include <unordered_set>

#include <rdkafkacpp.h>

#include "../utils/stringTools.h"

#include "./kafkaConfBuilder.h"

#include "../transport/io/IPublisher.h"
#include "../exceptions/argumentOutOfRangeException.h"
#include "../exceptions/invalidOperationException.h"
#include "./IKafkaPublisher.h"

namespace Quix { namespace Transport { namespace Kafka  {

class SubscriberConfiguration {
    std::map<std::string, std::string> consumerProperties_;

    /// NOTE: must reflect size of AutoOffsetReset
    const char* autoOffsetReset2Str[2] = {"earliest", "latest"};

public:
    /// The list of brokers as a comma separated list of broker host or host:port.
    const std::string brokerList;
    /// Whether the consumer group is set
    const bool consumerGroupSet;
    /// Client group id string. All clients sharing the same GroupId belong to the same group.
    const std::string groupId;
    /// Enable checking for alive messages for consumer
    bool checkForKeepAlivePackets = true;
    
    enum AutoOffsetReset {
        Unset = -1,
        Earliest = 0,
        Latest = 1
    };

    /**
     *      If consumer group is configured, The auto offset reset determines the start offset in the event
     *      there are not yet any committed offsets for the consumer group for the topic/partitions of interest.
     *      
     *  
     *      If no consumer group is configured, the consumption will start according to value set.
     *      If no auto offset reset is set, in case of no consumer group it defaults to end, otherwise to earliest.
     * 
     *      NOTE: if we support cpp17 -> use std::experimental::optional<>
    **/
    AutoOffsetReset autoOffsetReset = AutoOffsetReset::Unset;

    SubscriberConfiguration( std::string brokerList, std::string groupId = "", std::map<std::string, std::string> consumerProperties = std::map<std::string, std::string>() );


    KafkaConfBuilder::KafkaConfig* toConsumerConfig() const;

};


} } }