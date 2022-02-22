#pragma once

#include <string>
#include <sstream>
#include <map>
#include <unordered_set>

#include <librdkafka/rdkafkacpp.h>

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
     *      NOTE: if we support cpp17 -> use std::optional<>
    **/
    AutoOffsetReset autoOffsetReset = AutoOffsetReset::Unset;

    SubscriberConfiguration( std::string brokerList, std::string groupId = "", std::map<std::string, std::string> consumerProperties = std::map<std::string, std::string>() )
    :
    consumerProperties_( consumerProperties ),
    groupId( 
        Quix::StringTools::isEmptyOrWhiteSpace( groupId ) 
            ? 
            ( std::string("UNSET-") + Quix::StringTools::genUuid() )
            :
            groupId
    ),
    consumerGroupSet( !Quix::StringTools::isEmptyOrWhiteSpace( groupId ) ),
    brokerList( brokerList )
    {
        if( Quix::StringTools::isEmptyOrWhiteSpace( brokerList) )
        {
            throw ArgumentOutOfRangeException("brokerList Cannot be null or empty");
        }

        /// NOTE: this is done in toProducerConfig in C#, but here must be in constructor to allow toProducerConfig be const function 
        if ( consumerProperties_.find("log_level") == consumerProperties_.end() )
        {
            consumerProperties_["log_level"] = std::string("0");
        }
        if ( consumerProperties_.find("socket.keepalive.enable") == consumerProperties_.end() )
        {
            consumerProperties_["socket.keepalive.enable"] = std::string("true");
        }
        if ( consumerProperties_.find("metadata.max.age.ms") == consumerProperties_.end() )
        {
            consumerProperties_["metadata.max.age.ms"] = std::string("180000");
            // The hope here is that by refreshing metadata it is not considered idle
            // see more at https://docs.microsoft.com/en-us/azure/event-hubs/apache-kafka-configurations
        }
    }



    KafkaConfBuilder::KafkaConfig* toConsumerConfig() const
    {
        KafkaConfBuilder confBuilder;

        std::string errstr;

        for( auto& it : consumerProperties_ )
        {
            confBuilder.set( it.first.c_str(), it.second.c_str() );
        }

        confBuilder.set( "group.id", this->groupId.c_str() );
        confBuilder.set( "bootstrap.servers", this->brokerList.c_str() );

        const auto autoOffsetResetVal = autoOffsetReset != AutoOffsetReset::Unset ? autoOffsetReset : ( this->consumerGroupSet ? AutoOffsetReset::Earliest : AutoOffsetReset::Latest );
        confBuilder.set( "auto.offset.reset", autoOffsetReset2Str[ autoOffsetResetVal ] );
        confBuilder.set( "enable.auto.commit", false );

        return confBuilder.toConfig();
    }

};


} } }