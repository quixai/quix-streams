#pragma once

#include <string>
#include <map>

#include <librdkafka/rdkafkacpp.h>
#include "../transport/io/IPublisher.h"
#include "./IKafkaPublisher.h"

namespace Quix { namespace Transport { namespace Kafka  {

/**
 * Interface for providing a class a way to push Package to listener
 */
class PublisherConfiguration {

    std::string brokerList_; // The list of brokers as a comma separated list of broker host or host:port.
    std::map<std::string, std::string> producerProperties_;



public:

    /**
     * @brief Initializes a new instance of PublisherConfiguration
     * 
     * @param brokerList The list of brokers as a comma separated list of broker host or host:port.
     * @param producerProperties List of broker and producer kafka properties that overrides the default configuration values.
     */
    PublisherConfiguration( 
        std::string brokerList, 
        std::map<std::string, std::string> producerProperties = std::map<std::string, std::string>()
    );

    /**
     * @brief The list of brokers as a comma separated list of broker host or host:port.
     * 
     * @return std::string broker list
     */
    std::string brokerList() const
    {
        return brokerList_;
    }

    /**
     * @brief Maximum number of messages allowed on the queue.
     * default: 100000
     * negative means undefined
     */    
    int queueBufferingMaxMessages = -1; 

    /**
     * @brief Maximum total message size sum allowed on the queue. This property has higher priority than <see cref="QueueBufferingMaxMessages" />
     * default: 1048576
     * negative means undefined
     */
    int queueBufferingMaxKbytes = -1;

    /**
     *      Delay in milliseconds to wait for messages in the queue to accumulate before constructing message batches
     *      (MessageSets) to transmit to brokers. A higher value allows larger and more effective (less overhead, improved
     *      compression) batches of messages to accumulate at the expense of increased message delivery latency.
     *      default: 0.5
     *      Note: negative means undefined
    **/

    /**
     *      How many times to retry sending a failing Message. **Note:** retrying may cause reordering unless ` <see cref="EnableIdempotence" />` is set to true.
     *      default: 2
     *      Note: negative means undefined
    **/
    int messageSendMaxRetries = -1;

    /**
     *      Local message timeout. This value is only enforced locally and limits the time a produced message waits for
     *      successful delivery. A time of 0 is infinite. This is the maximum time librdkafka may use to deliver a message
     *      (including retries). Delivery error occurs when either the retry count or the message timeout are exceeded.
     *      default: 300000
     *      Note: negative means undefined
    **/
    int messageTimeoutMs = -1;

        // /// <summary>
        // ///     Partitioner: `random` - random distribution, `consistent` - CRC32 hash of key (Empty and NULL keys are mapped to
        // ///     single partition), `consistent_random` - CRC32 hash of key (Empty and NULL keys are randomly partitioned),
        // ///     `murmur2` - Java Producer compatible Murmur2 hash of key (NULL keys are mapped to single partition),
        // ///     `murmur2_random` - Java Producer compatible Murmur2 hash of key (NULL keys are randomly partitioned. This is
        // ///     functionally equivalent to the default partitioner in the Java Producer.).
        // ///     default: consistent_random
        // /// </summary>
        // public Partitioner? Partitioner { get; set; }

    /**
     *      When set to `true`, the producer will ensure that messages are successfully produced exactly once and in the
     *      original produce order. Producer instantiation will fail if user-supplied configuration (<see cref="MessageSendMaxRetries" /> &lt;= 0) is incompatible.
     *      default: false
    **/
    bool enableIdempotence = false;

    /// <summary>
    ///     Maximum Kafka protocol request message size in bytes.
    ///     default: 1000012
    /// </summary>
    int maxMessageSize = 1000012; // https://docs.confluent.io/current/installation/configuration/broker-configs.html


    /**
     *      The backoff time in milliseconds before retrying a protocol request.
     *      default: 100
     *      Note: negative means undefined
    **/
    int retryBackoffMs = -1;

    /**
     *      The threshold of outstanding not yet transmitted broker requests needed to backpressure the producer's message
     *      accumulator. If the number of not yet transmitted requests equals or exceeds this number, produce request creation
     *      that would have otherwise been triggered (for example, in accordance with <see cref="LingerMs" />) will be delayed.
     *      A lower number yields larger and more effective batches. A higher value can improve latency when using compression
     *      on slow machines.
     *      default: 1
     *      Note: negative means undefined
    **/
    int queueBufferingBackpressureThreshold = -1;

    /**
     *      Maximum number of messages batched in one MessageSet. The total MessageSet size is also limited by <see cref="MaxMessageSize" />.
     *      default: 10000
     *      Note: negative means undefined
    **/
    int BatchNumMessages = -1;
        
    /**
     *  Enable keep alive messages for publisher. Useful to ensure connection isn't idle reaped.
    **/
    bool keepConnectionAlive = true;

    /**
     *  The keel alive message interval in milliseconds
    **/
    int keepConnectionAliveInterval = 60000;


    /**
     * @brief Returns kafka producer configuration as a new object
     * 
     * @return new instance of the RdKafka::Conf*
     * Note: must be destroyed by delete sometime later to avoid memory leak
     */
    RdKafka::Conf* toProducerConfig() const;

};

} } }