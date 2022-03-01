#pragma once

#include <string>
#include <sstream>
#include <map>
#include <cctype>   //std::isspace
#include <algorithm>

#define __STDC_WANT_LIB_EXT1__ 1
#include <rdkafkacpp.h>

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
    static const Partition Any;

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

    bool operator!=(const Partition& other) const 
    {
        return !(*this == other);
    };


};

class PartitionOffset {
    Offset offset_;
    Partition partition_;

public:

    PartitionOffset( ) : offset_(), partition_() {};

    PartitionOffset( const Partition& partition, const Offset& offset )
    :
    partition_(partition),
    offset_(offset)
    {

    }

    PartitionOffset( const PartitionOffset& other ) = default;
    PartitionOffset& operator=( const PartitionOffset& other ) = default;

    inline bool operator<(const PartitionOffset& other) const
    {
        if( partition_ < other.partition_ )
        {
            return true;
        }
        if( offset_ < other.offset_ )
        {
            return true;
        }
        return false;
    }

    inline bool isUnset() const
    {
        return offset_ == Offset::Unset && partition_ == Partition::Unset;
    }

    const Offset offset() const
    {
        return offset_;
    }

    const Partition partition() const
    {
        return partition_;
    }

};

class TopicPartition {
    std::string topic_;
    Partition partition_;

public:

    TopicPartition( const std::string& topic, const Partition& partition )
    :
    topic_(topic),
    partition_(partition)
    {

    }

    inline bool operator<(const TopicPartition& other) const
    {
        if( topic_ < other.topic_ )
        {
            return true;
        }
        return partition_ < partition_ ;
    }

    inline bool operator==(const TopicPartition& other) const
    {
        return topic_ == other.topic_ && partition_ == other.partition_;
    }

    inline std::string topic() const
    {
        return topic_;
    }

    inline Partition partition() const
    {
        return partition_;
    }
};

class TopicPartitionOffset : public PartitionOffset {
    std::string topic_;
    PartitionOffset partitionOffset_;

public:

    TopicPartitionOffset(  )
    :
    topic_(""),
    partitionOffset_()
    {

    }

    TopicPartitionOffset( const std::string& topic, const PartitionOffset& partitionOffset )
    :
    partitionOffset_( partitionOffset ),
    topic_(topic)
    {        
    }

    TopicPartitionOffset( const TopicPartition& topicPartition, const Offset& offset )
    :
    partitionOffset_( PartitionOffset( topicPartition.partition(), offset ) ),
    topic_( topicPartition.topic() )
    {        
    }

    TopicPartitionOffset( const std::string& topic, const Partition& partition, const Offset& offset = Offset::Unset )
    :
    partitionOffset_( partition, offset ),
    topic_(topic)
    {        
    }

    TopicPartitionOffset( const std::string& topic, const int32_t partition, const Offset& offset = Offset::Unset )
    :
    partitionOffset_( Partition(topic, partition), offset ),
    topic_(topic)
    {        
    }

    TopicPartitionOffset( const TopicPartitionOffset& other ) = default;
    TopicPartitionOffset& operator=( const TopicPartitionOffset& other ) = default;

    TopicPartition topicPartition() const
    {
        return TopicPartition(topic_, partitionOffset_.partition());
    }

    

    inline bool operator<(const TopicPartitionOffset& other) const
    {
        if( topic_ < other.topic_ )
        {
            return true;
        }
        return partitionOffset_ < other.partitionOffset_ ;
    }

    inline bool isUnset() const
    {
        return this->partitionOffset_.isUnset() && topic_ == "";
    }

    inline PartitionOffset partitionOffset() const
    {
        return partitionOffset_;
    }

    inline std::string topic() const
    {
        return topic_;
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
        InputTopicConfiguration( std::string topic, Partition partition = Partition::Any )
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
            const auto key = tpo.topic();
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
                        return x.offset() == Offset::Unset; 
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
                return p.partition() == Partition::Any && p.offset() == Offset::Unset;
            } 
            ) == topicPartitionOffsets.end() 
        ) 
        {
            for( auto& el : topicPartitionOffsets )
            {
                this->topics.push_back( el.topic() );
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
        const std::vector<Partition>& partitions,
        const Offset& offset = Offset::Unset
    )
    :
    OutputTopicConfiguration( topic, OutputTopicConfiguration::addOffsetToPartitions( partitions, offset ) )
    {

    };

    /**
     *      Initializes a new <see cref="OutputTopicConfiguration" /> with a single topic and offset with default partition.
     *
     *  @param topic The topic to set the partitions for
     *  @param offset The offset to use
     */
    OutputTopicConfiguration( 
        const std::string& topic,
        const Offset& offset
    )
    :
    OutputTopicConfiguration( topic, std::vector<Partition>{ Partition::Any }, offset )
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