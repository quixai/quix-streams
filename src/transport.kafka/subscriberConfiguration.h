#pragma once

#include <string>
#include <sstream>
#include <map>

#include <librdkafka/rdkafkacpp.h>

#include "../utils/stringTools.h"

#include "../transport/io/IPublisher.h"
#include "../exceptions/argumentOutOfRangeException.h"
#include "../exceptions/invalidOperationException.h"
#include "./IKafkaPublisher.h"

namespace Quix { namespace Transport { namespace Kafka  {

class SubscriberConfiguration {
    bool consumerGroupSet_;
    std::string groupId_;

    std::map<std::string, std::string> consumerProperties_;

public:
    const std::string brokerList;

    SubscriberConfiguration( std::string brokerList, std::string groupId = "", std::map<std::string, std::string> consumerProperties = std::map<std::string, std::string>() )
    :
    consumerProperties_(consumerProperties),
    brokerList(brokerList)
    {
        if( Quix::StringTools::isEmptyOrWhiteSpace( brokerList) )
        {
            throw ArgumentOutOfRangeException("brokerList Cannot be null or empty");
        }
        if( Quix::StringTools::isEmptyOrWhiteSpace( groupId ) )
        {
            // means we're not using consumer group. In this case disallow use of commit
            consumerGroupSet_ = false;
            groupId = std::string("UNSET-") + Quix::StringTools::genUuid(); // technically any random static would do, but in case something does commit under that consumer id, it would possibly break things
        } else
        {
            consumerGroupSet_ = true;
        }



        this->groupId_ = groupId;

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

    RdKafka::Conf* toConsumerConfig() const
    {
        RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

        for( auto& it : consumerProperties_ )
        {
            std::string errstr;
            if( conf->set( it.first.c_str() , it.second.c_str(), errstr ) != RdKafka::Conf::CONF_OK )
            {
                std::stringstream ss;
                ss << "Failed assign kafka property (" << errstr << ")";
                throw InvalidOperationException(ss.str());
            }
        }

        return conf;
    }

};


} } }