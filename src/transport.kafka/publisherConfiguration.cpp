
#include "../exceptions/invalidOperationException.h"
#include "./publisherConfiguration.h"

#include <functional>
#include <sstream>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

PublisherConfiguration::PublisherConfiguration( 
    std::string brokerList, 
    std::map<std::string, std::string> producerProperties
)
: 
    brokerList_(brokerList),
    producerProperties_(producerProperties)
{

    /// NOTE: this is done in toProducerConfig in C#, but here must be in constructor to allow toProducerConfig be const function 
    if ( producerProperties_.find("log_level") == producerProperties_.end() )
    {
        producerProperties_["log_level"] = std::string("0");
    }

    if ( producerProperties_.find("compression.type") == producerProperties_.end() )
    {
        producerProperties_["compression.type"] = std::string("gzip"); // default to gzip
    }
        
    if ( producerProperties_.find("socket.keepalive.enable") == producerProperties_.end() )
    {
        producerProperties_["socket.keepalive.enable"] = std::string("true"); // default to true
    }

    /*
        https://github.com/edenhill/librdkafka/issues/3109 not yet implemented
    if (!producerProperties.ContainsKey("connections.max.idle.ms"))
    {
        producerProperties["connections.max.idle.ms"] = "180000"; // Azure closes inbound TCP idle > 240,000 ms, which can result in sending on dead connections (shown as expired batches because of send timeout)
        // see more at https://docs.microsoft.com/en-us/azure/event-hubs/apache-kafka-configurations
    }*/
    if ( producerProperties_.find("metadata.max.age.ms") == producerProperties_.end() )
    {
        producerProperties["metadata.max.age.ms"] = "180000"; // Azure closes inbound TCP idle > 240,000 ms, which can result in sending on dead connections (shown as expired batches because of send timeout)
        // The hope here is that by refreshing metadata it is not considered idle
        // see more at https://docs.microsoft.com/en-us/azure/event-hubs/apache-kafka-configurations
    }


};


RdKafka::Conf* PublisherConfiguration::toProducerConfig() const
{


    RdKafka::Conf* conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    for( auto& it : producerProperties_ )
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