
#include "./readMessage.h"

#include "utils/timer.h"
#include "transport/transportSubscriber.h"
#include "transport/registry/codecRegistry.h"
#include "transport.kafka/subscriberConfiguration.h"
#include "transport.kafka/topicConfiguration.h"

#include <iostream>


using namespace std;
using namespace Quix;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;


const Quix::Transport::BinaryCodec<ExampleModel> ReadMessage::codec1 = Quix::Transport::BinaryCodec<ExampleModel>();

ReadMessage::ReadMessage()
{

}

ReadMessage::~ReadMessage()
{
    if( statTimer_ != nullptr )
    {
        delete statTimer_;
    }
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
    hookupStatistics();
    subscriber->onNewPackage += std::bind( &ReadMessage::newMessageHandler, this, std::placeholders::_1 );
    kafkaSubscriber->open();

    this->registerCodecs();

    delete subConfig;
    delete topicConfig;

    return subscriber;
}

void ReadMessage::hookupStatistics()
{
    auto started = std::chrono::system_clock::now();

    statTimer_ = new CallbackTimer([this, started](){
        int published;
        {
            std::lock_guard<std::mutex> lock(this->lock_);
            published = this->subscribedCounter;
        }

        std::chrono::duration<double> elapsed = std::chrono::system_clock::now() - started;

        double elapsed_s = elapsed.count();
        
        auto publishedPerMin = (double)published * 60 / elapsed_s;

        cout << "Subscribed Messages: "<<published<<", "<<publishedPerMin<<"/min" << endl;
    }, Timer::UNSET, 1000, true);
}

void ReadMessage::newMessageHandler(std::shared_ptr<Quix::Transport::IPackage> package)
{
    std::lock_guard<std::mutex> lock(this->lock_);
    subscribedCounter++;
}


void ReadMessage::registerCodecs()
{
    auto registry = CodecRegistry::instance();
    registry->registerCodec( ModelKey::forType<ExampleModel>() , &codec1 );

}