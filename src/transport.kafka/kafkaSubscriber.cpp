
#include "./kafkaSubscriber.h"
#include "./exceptions/kafkaException.h"

#include <functional>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <string>


using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;

KafkaSubscriber::KafkaSubscriber( const SubscriberConfiguration& subscriberConfiguration )
{
    config_ = subscriberConfiguration.toConsumerConfig();
}


void KafkaSubscriber::commit(
    const std::vector<std::shared_ptr<TransportContext>>& transportContexts
)
{
    // TODO
}

std::vector<std::shared_ptr<TransportContext>> KafkaSubscriber::filterCommittedContexts(
    void* state, 
    const std::vector<std::shared_ptr<TransportContext>>& contextsToFilter
)
{
    // TODO
    return contextsToFilter;
}

std::vector<std::shared_ptr<TransportContext>> KafkaSubscriber::filterRevokedContexts(
    void* state, 
    const std::vector<std::shared_ptr<TransportContext>>& contexts
)
{
    // TODO
    return contexts;
}



void KafkaSubscriber::close( )
{

    if ( this->consumer_ == nullptr ) { return; }

    std::lock_guard<std::mutex> guard( consumerLock_ );

    auto& cons = this->consumer_;
    if ( cons == nullptr ) { return; }
    closing_ = true;
    lastRevokeCompleteAction_();

    // TODO
    // this.workerTaskCts?.Cancel()

    this->consumer_ = nullptr;


    try
    {
        // TODO
        // this.workerTaskPollFinished.Task?.Wait(-1); // length of the wait depends on how long each message takes to get processed
        // this.workerTask = null;
        // this.workerTaskPollFinished = null;
    }
    catch (std::exception ex)
    {
        // Any exception which happens here is related to worker task itself, not the processing of the msges
        // this.logger.LogDebug(ex, "WorkerTask failed when closing");
    }
    delete cons; // can't close before we're done returning from consumer.consume due to AccessViolationException, so
                    // while it would look like we could close consumer sooner than this, we can't really.
    closing_ = false;
}

void KafkaSubscriber::open()
{
    if ( this->consumer_ != nullptr ) { return; }

    std::lock_guard<std::mutex> guard( consumerLock_ );

    if ( this->consumer_ != nullptr ) { return; }
    closing_ = false;
    lastRevokeCancelAction_();
    // TODO
}

void KafkaSubscriber::commitOffsets()
{
    // TODO
}

void KafkaSubscriber::commitOffsets(const std::vector<TopicPartitionOffset>& offsets)
{
    // TODO
}

