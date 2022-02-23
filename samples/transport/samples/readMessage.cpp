
#include "./readMessage.h"

#include "transport/transportSubscriber.h"
#include "transport/registry/codecRegistry.h"
#include "transport.kafka/subscriberConfiguration.h"
#include "transport.kafka/topicConfiguration.h"

#include <iostream>

using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;


const Quix::Transport::BinaryCodec<ExampleModel> ReadMessage::codec1 = Quix::Transport::BinaryCodec<ExampleModel>();

ReadMessage::ReadMessage()
{

}

ISubscriber* ReadMessage::start( bool useConsumerGroup, Quix::Transport::Kafka::Offset offset )
{
    cout << "START READ MESSAGE" << endl;

    SubscriberConfiguration* subConfig;
    if( useConsumerGroup )
    {
        subConfig = new SubscriberConfiguration(Const::BrokerList, inputGroup);
    }
    else
    {
        subConfig = new SubscriberConfiguration(Const::BrokerList);
    }

    OutputTopicConfiguration* topicConfig;
    if( !(offset == Offset::Unset) )
    {
        topicConfig = new OutputTopicConfiguration(topicName, offset);
    }else
    {
        topicConfig = new OutputTopicConfiguration(topicName);
    }
    
    KafkaSubscriber* kafkaSubscriber = new KafkaSubscriber(*subConfig, *topicConfig);
    kafkaSubscriber->errorOccurred += [](const KafkaException& exception) { 
        cout << "Exception occurred" << endl;
    };

    TransportSubscriber* subscriber = new TransportSubscriber(kafkaSubscriber);
    // this.hookupStatistics();
    subscriber->onNewPackage += std::bind( &ReadMessage::newMessageHandler, this, std::placeholders::_1 );
    kafkaSubscriber->open();

    this->registerCodecs();

    return subscriber;
}

void ReadMessage::newMessageHandler(std::shared_ptr<Quix::Transport::IPackage> package)
{
    std::lock_guard<std::mutex> lock(this->lock_);
    consumerCounter++;
}


void ReadMessage::registerCodecs()
{
    auto registry = CodecRegistry::instance();
    registry->registerCodec( ModelKey::forType<ExampleModel>() , &codec1 );

}