#pragma once

#include <mutex>
#include <chrono>
#include <functional>
#include <memory>

#include <librdkafka/rdkafkacpp.h>

#include "../utils/timer.h"

#include "./IKafkaSubscriber.h"

#include "../transport/io/ISubscriber.h"
#include "../transport/fw/ICanCommit.h"
#include "../transport/fw/IRevocation.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/byteArray.h"
#include "./subscriberConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {


// typedef std::function<void(const std::string&, std::shared_ptr<Quix::Transport::IPackage>)> ProduceDelegate;
typedef std::function<void(const std::string&, const ByteArray&, void* state)> ProduceDelegate;


/**
 * Interface for providing a class a way to push Package to listener
 */
class KafkaSubscriber : public IKafkaSubscriber, public Quix::Transport::ISubscriber, public Quix::Transport::ICanCommit, public Quix::Transport::IRevocationPublisher{

    std::mutex consumerLock_;
    std::mutex workerThreadLock_;

    RdKafka::Consumer*  consumer_ = nullptr;
    RdKafka::Conf*      config_;

    bool canReconnect_ = true;
    bool closing_ = false;
    bool disconnected_ = false; // connection is deemed dead


    std::chrono::time_point<std::chrono::system_clock> lastReconnect_;
    std::chrono::duration<int, std::milli> minimumReconnectDelay_ = std::chrono::duration<int, std::milli>(30000);    // the absolute minimum time between two reconnect attempts


    const int revokeTimeoutPeriodInMs_ = 5000;

    bool consumerGroupSet_ = false; // higher number should only mean up to this seconds of delay if you are never going to get new assignments,
                                    // but should not prove to cause any other issue

    bool checkForKeepAlivePackets_ = true;   // Enables the check for keep alive messages from Quix

    std::function<void()> lastRevokeCompleteAction_ = [](){};
    std::function<void()> lastRevokeCancelAction_ = [](){};



public:

    /**
     *  Enables the custom re-assigned logic for when partition is revoked and then reassigned.
     *  If disabled, reverts to original kafka logic of always raising revoked partitions.
     *  Defaults to true.
    **/
    bool enableReAssignedLogic = true;


    /**
     * Commits the transport context to the output.
     * 
     * @param transportContexts The transport context to commit
     */
    void commit(const std::vector<std::shared_ptr<TransportContext>>& transportContexts);

    /**
     * @brief Filters contexts that were affected by the commit.
     * @param state State raised by onCommitted
     * @param contextsToFilter The contexts to filter
     * @return Contexts affected by the commit
     * 
     */
    std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter);

    /**
     * Filters contexts affected by the revocation.
     * 
     * @param state State raised as argument of onRevoking or onRevoked
     * @param contexts The contexts to filter
     * 
     * @return Contexts affected by the state
     */
    std::vector<std::shared_ptr<TransportContext>> filterRevokedContexts(void* state, const std::vector<std::shared_ptr<TransportContext>>& contexts);

    /**
     * Commits the transport context to the output.
     * 
     * @param transportContexts The transport context to commit
     */
    void close( );

    /**
     * @brief Open connection to Kafka
     */
    void open();

    /**
     * @brief Commits all offsets for the current topic partition assignments
     */
    void commitOffsets();

    /**
     * @brief Commit a list of offsets limited by the topics this output had previously subscribed to
     * 
     * @param offsets The offsets to commit
     */
    void commitOffsets(const std::vector<TopicPartitionOffset>& offsets);



    KafkaSubscriber( const SubscriberConfiguration& configuration );

    ~KafkaSubscriber();

};

} } }