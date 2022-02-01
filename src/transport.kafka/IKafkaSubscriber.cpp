
#include "./IKafkaSubscriber.h"
#include "./extensions/kafkaTransportContextExtensions.h"
#include "../utils/vectorTools.h"

using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;


void IKafkaSubscriber::commitOffset(const TopicPartitionOffset& transportContext)
{
    return this->commitOffsets(std::vector<TopicPartitionOffset>{ transportContext });
}

std::vector<TopicPartitionOffset> IKafkaSubscriber::getPartitionOffsets( 
    const std::vector<std::shared_ptr<TransportContext>>& transportContexts
)
{
    std::vector<TopicPartitionOffset> ret;

    for( auto& transportContext : transportContexts )
    {
        if ( transportContext.get() == nullptr ) // This should no longer
        {
            // logger.Value.LogDebug("Transport context {0} of {1} is null during execution of GetPartitionOffsets.", index, transportContexts.Length);
            // if (includeInvalidAsNull) yield return null;
            continue;
        };

        std::string topic;
        int partition;
        int64_t offset;
        if ( !KafkaTransportContextExtensions( transportContext.get() ).tryGetKafkaCommitDetails( topic, partition, offset ) )
        {
            // if (includeInvalidAsNull) yield return null;
            continue;
        }
        ret.push_back( TopicPartitionOffset( topic, partition, offset ) );
    }

    return ret;
}


void IKafkaSubscriber::commitOffsetsInternal(
    const std::vector<std::shared_ptr<TransportContext>>& transportContexts
)
{
    if( transportContexts.size() <= 0 )
    {
        return;
    }

    const int offSetToNextPackage = 1;
    if( transportContexts.size() == 1 )
    {
        auto& transportContext = transportContexts[0];

        std::string topic;
        int partition;
        int64_t offset;
        if ( !KafkaTransportContextExtensions( transportContext.get() ).tryGetKafkaCommitDetails(topic, partition, offset) ) { return; };
        this->commitOffset( TopicPartitionOffset(topic, partition, offset + offSetToNextPackage) );

        return;
    }

    auto offsets = IKafkaSubscriber::getPartitionOffsets( transportContexts );

    /// find max offset for topicPartition
    vector<TopicPartitionOffset> latestOffsets;
    auto groupped = VectorTools<Quix::Transport::Kafka::TopicPartitionOffset, Quix::Transport::Kafka::TopicPartitionOffset, int64_t>::groupBy(
        offsets, 
        []( 
            const Quix::Transport::Kafka::TopicPartitionOffset& tpo 
        ){ 
            return Quix::Transport::Kafka::TopicPartitionOffset(tpo.topic, tpo.partition, Offset::Unset); 
        },
        []( 
            const Quix::Transport::Kafka::TopicPartitionOffset& tpo 
        ){ 
            return tpo.offset.value;
        }
    );
    for( auto it = groupped.begin(); it != groupped.end(); ++it )
    {
        auto& offsets = it->second;
        auto maxOffset = *std::max_element(std::begin(offsets), std::end(offsets)) + offSetToNextPackage;
        auto& tpo = it->first;
        latestOffsets.push_back( TopicPartitionOffset( tpo.topic, tpo.partition.id, maxOffset ) );
    }
    this->commitOffsets( latestOffsets );    
}
