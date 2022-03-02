
#include "../exceptions/invalidOperationException.h"
#include "./publisherConfiguration.h"

#include "./kafkaConfBuilder.h"

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

    // TODO: enable gzip in kafka build
    // if ( producerProperties_.find("compression.type") == producerProperties_.end() )
    // {
    //     producerProperties_["compression.type"] = std::string("gzip"); // default to gzip
    // }
        
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


KafkaConfBuilder::KafkaConfig* PublisherConfiguration::toProducerConfig() const
{
    KafkaConfBuilder confBuilder;

    for( auto& it : producerProperties_ )
    {
        confBuilder.set( it.first.c_str(), it.second.c_str() );
    }

    confBuilder.set( "bootstrap.servers", this->brokerList().c_str() );

    // Queue buffering
    if( this->queueBufferingMaxKbytes >= 0 ) {
        confBuilder.set( "queue.buffering.max.kbytes", this->queueBufferingMaxKbytes );
    }
    if( this->queueBufferingMaxKbytes >= 0 ) {
        confBuilder.set( "queue.buffering.max.messages", this->queueBufferingMaxMessages );
    }
    if( this->queueBufferingBackpressureThreshold >= 0 ) {
        confBuilder.set( "queue.buffering.backpressure.threshold", this->queueBufferingBackpressureThreshold );
    }
    // queue buffering
    if( this->lingerMs >= 0 ) {
        confBuilder.set( "linger.ms", this->lingerMs );
    }

    if( this->partitioner != Partitioner::Unset ) {
        confBuilder.set( "partitioner", partitioner2str[ this->partitioner ] );
    }    
    if( this->BatchNumMessages >= 0 ) {
        confBuilder.set( "batch.num.messages", this->BatchNumMessages );
    }
    confBuilder.set( "enable.idempotence", this->enableIdempotence );
    if( this->maxMessageSize >= 0 ) {
        confBuilder.set( "message.max.bytes", this->maxMessageSize );
    }
    if( this->retryBackoffMs >= 0 ) {
        confBuilder.set( "retry.backoff.ms", this->retryBackoffMs );
    }
    if( this->messageTimeoutMs >= 0 ) {
        confBuilder.set( "message.timeout.ms", this->messageTimeoutMs );
    }
    if( this->messageSendMaxRetries >= 0 ) {
        confBuilder.set( "message.send.max.retries", this->messageSendMaxRetries );
    }

    return confBuilder.toConfig();
}