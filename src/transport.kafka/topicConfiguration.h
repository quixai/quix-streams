#pragma once

#include <string>
#include <sstream>
#include <map>
#include <cctype>   //std::isspace
#include <algorithm>

#include <librdkafka/rdkafkacpp.h>

#include "../exceptions/argumentOutOfRangeException.h"
#include "../utils/stringTools.h"

namespace Quix { namespace Transport { namespace Kafka  {


class Offset {
    int64_t value_;
public:
    Offset(int64_t offset = RdKafka::Topic::OFFSET_STORED-1)
    :
    value_(offset)
    {

    }

    static const Offset Beginning;
    static const Offset End;
    static const Offset Stored;
    static const Offset Unset;

    Offset( const Offset& other ) = default;
    Offset& operator=( const Offset& other ) = default;


    bool operator==(const Offset& other )const
    {
        return value_ == other.value_;
    }

    bool operator<(const Offset& other) const
    {
        return value_ < other.value_;
    }

    int64_t value() const{ return value_; }

    bool isSpecial() const;

};

class Partition{

public:

    int32_t id;
    std::string topic;

    static const Partition Unset;

    Partition(std::string topic_, int32_t id_ = 0)
    :
    id(id_),
    topic(topic_)
    {
    }

    Partition( ) = default;
    Partition( const Partition& other ) = default;
    Partition& operator=( const Partition& other ) = default;

    bool operator==(const Partition& other) const 
    {
        return 
            id == other.id 
            &&
            topic == other.topic
            ;        
    };

    bool operator<(const Partition& other) const
    {
        if( id < other.id )
        {
            return true;
        }
        if( topic < other.topic )
        {
            return true;
        }
        return false;
    }


    static Partition Any;

};

class PartitionOffset {
public:
    const Offset offset;
    const Partition partition;

    PartitionOffset( ) : offset(), partition() {};

    PartitionOffset( const Partition& partition, const Offset& offset )
    :
    partition(partition),
    offset(offset)
    {

    }

    PartitionOffset( const PartitionOffset& other ) = default;
    PartitionOffset& operator=( const PartitionOffset& other ) = default;

    inline bool operator<(const PartitionOffset& other) const
    {
        if( partition < other.partition )
        {
            return true;
        }
        if( offset < other.offset )
        {
            return true;
        }
        return false;
    }

    inline bool isUnset() const
    {
        return offset == Offset::Unset && partition == Partition::Unset;
    }

};

class TopicPartition {
public:
    const std::string topic;
    const Partition partition;

    TopicPartition( const std::string& topic, const Partition& partition )
    :
    topic(topic),
    partition(partition)
    {

    }

    inline bool operator<(const TopicPartition& other) const
    {
        if( topic < other.topic )
        {
            return true;
        }
        return partition < partition ;
    }

    inline bool operator==(const TopicPartition& other) const
    {
        return topic == other.topic && partition == other.partition;
    }
};

class TopicPartitionOffset : public PartitionOffset {
public:
    const std::string topic;
    const PartitionOffset partitionOffset;

    TopicPartitionOffset(  )
    :
    topic(""),
    partitionOffset()
    {

    }

    TopicPartitionOffset( const std::string& topic, const PartitionOffset& partitionOffset )
    :
    partitionOffset( partitionOffset ),
    topic(topic)
    {        
    }

    TopicPartitionOffset( const TopicPartition& topicPartition, const Offset& offset )
    :
    partitionOffset( PartitionOffset( topicPartition.partition, offset ) ),
    topic( topicPartition.topic )
    {        
    }

    TopicPartitionOffset( const std::string& topic, const Partition& partition, const Offset& offset = Offset::Unset )
    :
    partitionOffset( partition, offset ),
    topic(topic)
    {        
    }

    TopicPartitionOffset( const std::string& topic, const int32_t partition, const Offset& offset = Offset::Unset )
    :
    partitionOffset( Partition(topic, partition), offset ),
    topic(topic)
    {        
    }

    TopicPartitionOffset( const TopicPartitionOffset& other ) = default;
    TopicPartitionOffset& operator=( const TopicPartitionOffset& other ) = default;

    TopicPartition topicPartition() const
    {
        return TopicPartition(topic, partitionOffset.partition);
    }

    inline bool operator<(const TopicPartitionOffset& other) const
    {
        if( topic < other.topic )
        {
            return true;
        }
        return partitionOffset < other.partitionOffset ;
    }

    inline bool isUnset() const
    {
        return this->partitionOffset.isUnset() && topic == "";
    }

};




/**
 * Topic configuration for Kafka Input
 */
class InputTopicConfiguration{
    
    std::string topic_;
    Partition partition_;

public:

        /***
         * @brief Initializes a new instance of <see cref="InputTopicConfiguration"/>
         * 
         * @param topic The topic to write to
         * @param partition The partition to write to
         **/
        InputTopicConfiguration(std::string topic, Partition partition)
        :
         topic_(topic),
         partition_(partition)
        {
            // Check if topic consists only of whitespaces
            if ( Quix::StringTools::isEmptyOrWhiteSpace(topic) )
            {
                throw ArgumentOutOfRangeException("topic Cannot be null or empty");
            }

            // TODO: partition
            // this.Partition = partition;
        }

        /**
         * @brief The topic to write to
         * 
         * @return std::string 
         */
        std::string topic() const { return topic_; }

        /**
         * @brief The topic to write to
         * 
         * @return std::string 
         */
        Partition partition() const { return partition_; }

};


/**
 * Interface for providing a class a way to push Package to listener
 */
class OutputTopicConfiguration {

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

    OutputTopicConfiguration( const std::vector<TopicPartitionOffset>& topicPartitionOffsets )
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
    OutputTopicConfiguration( const std::string& topic, const std::vector<PartitionOffset>& partitionOffsets )
    :
    OutputTopicConfiguration( OutputTopicConfiguration::addTopicToPartitionOffsets(topic, partitionOffsets) )
    {

    }

    /**
     *      Initializes a new <see cref="OutputTopicConfiguration" /> for one or more topics where partitions will be automatically 
     *      selected and offset will be the last unread offset or first available
     *      offset if no previous offset for consumer group is found.
     * 
     * @param topics The topics
     */
    OutputTopicConfiguration( 
        const std::string& topic,
        const std::vector<Partition>& partitions = std::vector<Partition>{ Partition::Any },
        const Offset& offset = Offset::Unset
    )
    :
    OutputTopicConfiguration( topic, OutputTopicConfiguration::addOffsetToPartitions( partitions, offset ) )
    {

    };

    /**
     *      Initializes a new <see cref="OutputTopicConfiguration" /> for a single topic where partitions will be automatically 
     *      selected and offset will be the last unread offset or first available offset if
     *      no previous offset for consumer group is found.
     * 
     * @param topic The topic
     */
    OutputTopicConfiguration( 
        std::string topic,
        const Partition& partition = Partition::Any,
        const Offset& offset = Offset::Unset
    )
     :
     OutputTopicConfiguration( topic, std::vector<Partition>{ partition }, offset)
    {

    };



};


} } }