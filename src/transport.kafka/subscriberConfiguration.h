#pragma once

#include <string>
#include <sstream>
#include <map>

#include <librdkafka/rdkafkacpp.h>
#include "../transport/io/IPublisher.h"
#include "../exceptions/argumentOutOfRangeException.h"
#include "./IKafkaPublisher.h"

namespace Quix { namespace Transport { namespace Kafka  {

class Offset {
public:
    static const Offset Unset;

    bool operator==(const Offset& other )const
    {
        //TODO
        return true;
    };
};

class Partition {
public:
    static const Partition Any;

    bool operator==(const Partition& other )const
    {
        //TODO
        return true;
    };
};

class PartitionOffset {
public:
    const Offset offset;
    const Partition partition;

    PartitionOffset( const Partition& partition, const Offset& offset )
    :
    partition(partition),
    offset(offset)
    {

    }
};

class TopicPartitionOffset : public PartitionOffset {
public:
    const std::string topic;

    TopicPartitionOffset( const std::string& topic, const PartitionOffset& partitionOffset )
    :
    PartitionOffset( partitionOffset ),
    topic(topic)
    {        
    }


    TopicPartitionOffset( const std::string& topic, const Partition& partition, const Offset& offset )
    :
    PartitionOffset( partition, offset ),
    topic(topic)
    {        
    }
};

/**
 * Interface for providing a class a way to push Package to listener
 */
class SubscriberConfiguration {

    static std::vector<TopicPartitionOffset> addTopicToPartitionOffsets
    ( 
        const std::string& topic, 
        const std::vector<PartitionOffset>& partitionOffsets 
    ) {
        std::vector<TopicPartitionOffset> inp;
        for( auto& el : partitionOffsets )
        {
            inp.push_back( TopicPartitionOffset( topic, el ) );
        }
        return inp;
    }

    static std::vector<PartitionOffset> addOffsetToPartitions(
        const std::vector<Partition>& partitions,
        const Offset& offset
    )
    {
        std::vector<PartitionOffset> inp;
        for( auto& el : partitions )
        {
            inp.push_back( PartitionOffset( el, offset ) );
        }
        return inp;
    }

public:

    //TODO: make const
    std::vector<std::string> topics;
    std::vector<TopicPartitionOffset> partitions;

    /**
     *  @brief Initializes a new <see cref="OutputTopicConfiguration" /> with multiple topics where each topic has one or more configured partition offset
     *  
     *  @param topicPartitionOffsets The topics with partition offsets
     **/

    SubscriberConfiguration( const std::vector<TopicPartitionOffset>& topicPartitionOffsets )
    {
        if ( topicPartitionOffsets.size() <= 0 )
        {
            throw ArgumentOutOfRangeException("topicPartitionOffsets cannot be empty");
        }

        std::map<std::string, std::vector<TopicPartitionOffset>> groupedTopicPartitions;
        for( auto& tpo : topicPartitionOffsets )
        {
            auto& key = tpo.topic;
            if( groupedTopicPartitions.find( key ) == groupedTopicPartitions.end() )
            {
                groupedTopicPartitions[key] = std::vector<TopicPartitionOffset>( );
            }
            groupedTopicPartitions[key].push_back(tpo);
        }

        for( auto& partitionOffsets : groupedTopicPartitions )
        {
            if( 
                partitionOffsets.second.size() != 1 
                && 
                // TODO: little different here from the Csharp
                find_if( 
                    partitionOffsets.second.begin(), 
                    partitionOffsets.second.end(), 
                    [](
                        const TopicPartitionOffset& x
                    ){ 
                        return x.offset == Offset::Unset; 
                    } 
                )  != partitionOffsets.second.end()
            )
            {
                std::stringstream ss;
                ss << "";
                throw new ArgumentOutOfRangeException("Provided multiple partition values for {partitionOffsets.Key} where at least one is Any. Should provide only Any or multiple without Any. ");
            }   
        }

        // check if it is a simple subscription to topics
        if ( 
            find_if_not( 
                topicPartitionOffsets.begin(), 
                topicPartitionOffsets.end(), 
            [](
                const TopicPartitionOffset& p
            ){
                return p.partition == Partition::Any && p.offset == Offset::Unset;
            } 
            ) == topicPartitionOffsets.end() 
        ) 
        {
            for( auto& el : topicPartitionOffsets )
            {
                this->topics.push_back( el.topic );
            }
            return;
        }

        // no, it is not a simple topic subscription
        for( auto& el : topicPartitionOffsets )
        {
            this->partitions.push_back( el );
        }
    }

    /**
     * @brief Initializes a new <see cref="OutputTopicConfiguration" /> with a single topic with the specified partition offsets.
     * 
     * @param topic The topic to set the partitions for
     * @param partitionOffsets The partitions with offsets to listen to
     **/
    SubscriberConfiguration( const std::string& topic, const std::vector<PartitionOffset>& partitionOffsets )
    :
    SubscriberConfiguration( SubscriberConfiguration::addTopicToPartitionOffsets(topic, partitionOffsets) )
    {

    }

    /**
     *      Initializes a new <see cref="OutputTopicConfiguration" /> for one or more topics where partitions will be automatically 
     *      selected and offset will be the last unread offset or first available
     *      offset if no previous offset for consumer group is found.
     * 
     * @param topics The topics
     */
    SubscriberConfiguration( 
        const std::string& topic,
        const std::vector<Partition>& partitions = std::vector<Partition>{ Partition::Any },
        const Offset& offset = Offset::Unset
    )
    :
    SubscriberConfiguration( topic, SubscriberConfiguration::addOffsetToPartitions( partitions, offset ) )
    {

    };

    /**
     *      Initializes a new <see cref="OutputTopicConfiguration" /> for a single topic where partitions will be automatically 
     *      selected and offset will be the last unread offset or first available offset if
     *      no previous offset for consumer group is found.
     * 
     * @param topic The topic
     */
    SubscriberConfiguration( 
        std::string topic,
        const Partition& partition = Partition::Any,
        const Offset& offset = Offset::Unset
    )
     :
     SubscriberConfiguration( topic, std::vector<Partition>{ partition }, offset)
    {

    };



};

} } }