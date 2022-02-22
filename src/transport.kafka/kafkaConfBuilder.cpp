#include <sstream>
#include <string>

#include <librdkafka/rdkafkacpp.h>

#include "../exceptions/argumentOutOfRangeException.h"
#include "./kafkaConfBuilder.h"

using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaConfBuilder::KafkaConfBuilder() 
:
conf_(RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL))
{
};

/// Internal function to set key with specific value into the Rdkafak conf 
KafkaConfBuilder& KafkaConfBuilder::set( const char* key, const char* value )
{
    std::string errstr;
    if( conf_->set( key , value, errstr ) != RdKafka::Conf::CONF_OK )
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

RdKafka::Conf* KafkaConfBuilder::toConfig() const
{
    return conf_;
}


