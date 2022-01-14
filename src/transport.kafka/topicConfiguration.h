#pragma once

#include <string>
#include <cctype>   //std::isspace
#include <algorithm>

#include "../exceptions/argumentOutOfRangeException.h"

namespace Quix { namespace Transport { namespace Kafka  {


class Partition{

public:

    int id = 0;

    bool operator==(const Partition& other) const 
    {
        return id == other.id;        
    };

    static Partition Any;

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
            bool topicIsWhiteSpacesOnly = std::all_of(topic.begin(),topic.end(),[](unsigned char c){ return std::isspace(c); });
            if ( topic.size() <= 0 || topicIsWhiteSpacesOnly )
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

} } }