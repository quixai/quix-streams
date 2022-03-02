#pragma once

#include <rdkafkacpp.h>

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 * 
 * Note: this Class creates new object RdKafka::Conf in the constructor, 
 *    you must call toConfig() and delete the object once it is no longer used
 */
class KafkaConfBuilder {

    RdKafka::Conf* conf_; //global ( non-topic specific ) configuration
    RdKafka::Conf* topicConf_; // topic specific configuration

    

public:

    class KafkaConfig {
        RdKafka::Conf* global_;
        RdKafka::Conf* topic_;
    public:

        KafkaConfig(RdKafka::Conf* global, RdKafka::Conf* topic)
        : global_(global), topic_(topic) 
        {

        };
        KafkaConfig(const KafkaConfig& config) = default;

        ~KafkaConfig()
        {
            delete global_;
            delete topic_;
        };


        // returns global ( non-topic specific ) RdKafka::conf configuration 
        RdKafka::Conf* global() const { return global_; };
        // returns topic specific RdKafka::conf configuration 
        RdKafka::Conf* topic() const { return topic_; };

    };

    KafkaConfBuilder( );

    /// Internal function to set key with specific value into the Rdkafak conf 
    KafkaConfBuilder& set( const char* key, const char* value );
    /// Internal function to set key with specific value into the Rdkafak conf 
    KafkaConfBuilder& set( const char* key, double value );
    /// Internal function to set key with specific value into the Rdkafak conf 
    KafkaConfBuilder& set( const char* key, int value );
    /// Internal function to set key with specific value into the Rdkafak conf 
    KafkaConfBuilder& set( const char* key, long long value );
    /// Internal function to set key with specific value into the Rdkafak conf 
    KafkaConfBuilder& set( const char* key, bool value );

    KafkaConfig* toConfig() const;

};


} } }