#include "./topicConfiguration.h"

using namespace Quix::Transport::Kafka;

const Offset Offset::Beginning = Offset(RdKafka::Topic::OFFSET_BEGINNING);
const Offset Offset::End = Offset(RdKafka::Topic::OFFSET_END);
const Offset Offset::Stored = Offset(RdKafka::Topic::OFFSET_STORED);
const Offset Offset::Unset = Offset(RdKafka::Topic::OFFSET_STORED-1);

bool Offset::isSpecial() const 
{
    return 
        this->offset == RdKafka::Topic::OFFSET_BEGINNING
        ||
        this->offset == RdKafka::Topic::OFFSET_END
        ||
        this->offset == RdKafka::Topic::OFFSET_STORED
        ||
        this->offset == (RdKafka::Topic::OFFSET_STORED-1)
        ;
}

