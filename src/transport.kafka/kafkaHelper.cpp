#include <mutex>
#include <sstream>
#include "./kafkaHelper.h"

#include "../transport/io/ISubscriber.h"
#include "../transport/io/package.h"
#include "../transport/io/IPackage.h"
#include "../transport/io/byteArray.h"
#include "../transport/io/transportContext.h"
#include "./subscriberConfiguration.h"
#include "./topicConfiguration.h"

#include "./knownKafkaTransportContextKeys.h"

using namespace Quix::Transport::Kafka;
using namespace Quix::Transport;
using namespace std;

std::shared_ptr<ByteArrayPackage> KafkaHelper::fromResult( RdKafka::Message *message )
{
    auto transportContext = std::shared_ptr<TransportContext>( new TransportContext() );
    (*transportContext)[KnownTransportContextKeys::MessageGroupKey] = *(message->key());
    (*transportContext)[KnownKafkaTransportContextKeys::Topic]  = message->topic_name();
    (*transportContext)[KnownKafkaTransportContextKeys::Key]    = *(message->key());
    (*transportContext)[KnownKafkaTransportContextKeys::Partition]  = message->partition();
    (*transportContext)[KnownKafkaTransportContextKeys::Offset] = message->offset();
    (*transportContext)[KnownKafkaTransportContextKeys::DateTime] = message->timestamp().timestamp;
    return std::shared_ptr<ByteArrayPackage>(
                        new ByteArrayPackage(
                            ByteArray::initFromArray((uint8_t*)message->payload(), message->len()),
                            transportContext
                        )
                    );
}
