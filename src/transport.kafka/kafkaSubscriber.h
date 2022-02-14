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
#include "./topicConfiguration.h"

namespace Quix { namespace Transport { namespace Kafka  {

class ConsumerResult { };

// typedef std::function<void(const std::string&, std::shared_ptr<Quix::Transport::IPackage>)> ProduceDelegate;
typedef std::function<void(const std::string&, const ByteArray&, void* state)> ProduceDelegate;
typedef std::function<bool(const ConsumerResult&)> ShouldSkipConsumeResult;

class TopicPartitionOffsetError
{
public:
    const KafkaException error;
    const TopicPartitionOffset topicPartitionOffset;
    TopicPartitionOffsetError(const TopicPartitionOffset& offset, const KafkaException& error)
    :
    topicPartitionOffset(offset),
    error(error)
    {

    }
};

class CommittedOffsets
{
public:
    const KafkaException error;
    const std::vector<TopicPartitionOffsetError> offsets;
    CommittedOffsets(const std::vector<TopicPartitionOffsetError>& offsets, const KafkaException& error)
    :
    offsets(offsets),
    error(error)
    {

    }
};

/**
 * Interface for providing a class a way to push Package to listener
 */
class KafkaSubscriber : public IKafkaSubscriber, public Quix::Transport::ISubscriber, public Quix::Transport::ICanCommit, public Quix::Transport::IRevocationPublisher{

    std::mutex consumerLock_;
    std::mutex workerThreadLock_;

    RdKafka::KafkaConsumer*  consumer_ = nullptr;
    RdKafka::Conf*      config_;

    std::thread readThread_;
    std::mutex threadLock_;
    bool threadShouldBeRunning_ = true;

    const OutputTopicConfiguration topicConfiguration_;

    bool canReconnect_ = true;
    bool closing_ = false;
    bool disconnected_ = false; // connection is deemed dead

    ShouldSkipConsumeResult seekFunc = [](const ConsumerResult&){ return false; };


    std::chrono::time_point<std::chrono::system_clock> lastReconnect_;
    std::chrono::duration<int, std::milli> minimumReconnectDelay_ = std::chrono::duration<int, std::milli>(30000);    // the absolute minimum time between two reconnect attempts


    const int revokeTimeoutPeriodInMs_ = 5000;

    const bool consumerGroupSet_ = false; // higher number should only mean up to this seconds of delay if you are never going to get new assignments,
                                    // but should not prove to cause any other issue

    bool checkForKeepAlivePackets_ = true;   // Enables the check for keep alive messages from Quix

    std::vector<TopicPartitionOffset> lastRevokingState;
    std::function<void()> lastRevokeCompleteAction_ = [](){};
    std::function<void()> lastRevokeCancelAction_ = [](){};

    class KafkaEventCb : public RdKafka::EventCb {
        KafkaSubscriber* subscriber_;
    public:
        KafkaEventCb( KafkaSubscriber* subscriber );
        void event_cb(RdKafka::Event &event);
    };

    class KafkaRebalanceCb : public RdKafka::RebalanceCb {
    private:
        KafkaSubscriber* parent_;
        static void part_list_print(
            const std::vector<RdKafka::TopicPartition *> &partitions) {
            for (unsigned int i = 0; i < partitions.size(); i++)
                std::cerr << partitions[i]->topic() << "[" << partitions[i]->partition()
                            << "], ";
            std::cerr << "\n";
        }
    public:
        KafkaRebalanceCb(KafkaSubscriber* parent) : parent_(parent) {};
        void rebalance_cb(RdKafka::KafkaConsumer *consumer,
                            RdKafka::ErrorCode err,
                            std::vector<RdKafka::TopicPartition *> &partitions);
    };

    class KafkaOffsetCommitCb : public RdKafka::OffsetCommitCb {
        KafkaSubscriber* parent_;
    public:
        KafkaOffsetCommitCb(KafkaSubscriber* parent) : parent_(parent) {};
        void offset_commit_cb(RdKafka::ErrorCode err,
                                        std::vector<RdKafka::TopicPartition*>&offsets);
    };


    KafkaEventCb eventCallback_;
    KafkaRebalanceCb rebalanceCallback_;

    void addMessage( RdKafka::Message *message  );
    void kafkaPollingThread();
    void startWorkerThread();

    void onErrorOccurred(const KafkaException& exception);

    CommittedOffsets getCommittedOffsets( const std::vector<TopicPartitionOffset>& offsets ) const;

    void partitionsAssignedHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions);
    void partitionsLostHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions);
    void partitionsRevokedHandler(RdKafka::KafkaConsumer *consumer, const std::vector< RdKafka::TopicPartition * >& partitions);
    void automaticOffsetCommitedHandler(const std::vector< RdKafka::TopicPartition * >& partitions, RdKafka::ErrorCode err);

    std::vector<TopicPartitionOffset> createTopicPartitionOffsetList(const std::vector< RdKafka::TopicPartition * >&)const;


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
    std::vector<std::shared_ptr<TransportContext>> filterCommittedContexts(Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter);

    /**
     * Filters contexts affected by the revocation.
     * 
     * @param state State raised as argument of onRevoking or onRevoked
     * @param contexts The contexts to filter
     * 
     * @return Contexts affected by the state
     */
    std::vector<std::shared_ptr<TransportContext>> filterRevokedContexts(const Quix::Object* state, const std::vector<std::shared_ptr<TransportContext>>& contexts);

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



    KafkaSubscriber( const SubscriberConfiguration& configuration, const OutputTopicConfiguration& topicConfiguration );

    ~KafkaSubscriber();

};

} } }