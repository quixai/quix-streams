#include <sstream>
#include <string>

#include <rdkafkacpp.h>

#include "../exceptions/argumentOutOfRangeException.h"
#include "./kafkaConfBuilder.h"

using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaConfBuilder::KafkaConfBuilder( ) 
:
conf_( RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL) ),
topicConf_( RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC) )
{   
};

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, const char* value )
{
    std::string errstr;
    RdKafka::Conf::ConfResult res;
    if ( !strncmp(key, "topic.", strlen("topic.")) )
    {
        res = topicConf_->set( key + strlen("topic.") , value, errstr );
    } else {
        res = conf_->set( key , value, errstr );
    }

    if( res != RdKafka::Conf::CONF_OK )
    {
        std::stringstream ss;
        ss << "Failed assign kafka " << key << ">>>> property (" << errstr << ")";
        throw ArgumentOutOfRangeException( ss.str() );
    }

    return *this;
}

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, int value )
{
    stringstream ss;
    ss << value << flush;
    return set( key, ss.str().c_str() );
}

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, long long value )
{
    stringstream ss;
    ss << value << flush;
    return set( key, ss.str().c_str() );
}

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, double value )
{
    stringstream ss;
    ss << value << flush;
    return set( key, ss.str().c_str() );
}

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, bool value )
{
    return set( key, value ? "true" : "false" );
}

KafkaConfBuilder::KafkaConfig* KafkaConfBuilder::toConfig() const
{
    return new KafkaConfig(conf_, topicConf_);
}