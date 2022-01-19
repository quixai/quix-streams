
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

KafkaSubscriber::KafkaSubscriber( const SubscriberConfiguration& configuration )
:
configuration(configuration)
{

}
