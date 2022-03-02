
#include "./subscriberConfiguration.h"

#include <functional>
#include <sstream>
#include <algorithm>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;



SubscriberConfiguration::SubscriberConfiguration( std::string brokerList, std::string groupId, std::map<std::string, std::string> consumerProperties )
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


KafkaConfBuilder::KafkaConfig* SubscriberConfiguration::toConsumerConfig() const
{
    KafkaConfBuilder confBuilder;

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
