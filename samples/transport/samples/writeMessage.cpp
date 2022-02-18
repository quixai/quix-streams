
#include "./writeMessage.h"

#include "../utils/ptrLock.h"

#include "transport.kafka/kafkaPublisher.h"
#include "transport/transportPublisher.h"

#include <iostream>

#include <mutex>

using namespace std;
using namespace Quix::Transport;
using namespace Quix::Transport::Kafka;


void WriteMessage::run()
{
    cout << "START WRITE MESSAGE" << endl;


    // ByteSplitter splitter;
    ByteSplitter splitter(1000);

    auto publisher = this->createPublisher( splitter );

    {
        PtrLock<IKafkaPublisher> lock(publisher);

        TransportPublisher transportPublisher(publisher, &splitter);

        // this->hookupStatistics();        

        try
        {
            this->sendMessage(&transportPublisher);
        }
        catch ( ... )
        {
            cerr << "Exception in main writer thread " << endl;
            // Console.WriteLine(ex);
        }


    }


    cout << "END WRITE MESSAGE" << endl;
}


void WriteMessage::sendMessage( IPublisher* publisher )
{
    int counter = 0;


    while( doWork_ )
    {
        auto currentCounter = counter;

        auto package = ByteArrayPackage( ByteArray::initRandom( this->messageSizeInBytes ) );

    }
    
}



Quix::Transport::Kafka::IKafkaPublisher* WriteMessage::createPublisher(Quix::Transport::ByteSplitter& byteSplitter)
{
    PublisherConfiguration pubConfig(Const::BrokerList);
    pubConfig.maxMessageSize = this->maxMessageSizeInKafka;

    byteSplitter = ByteSplitter( pubConfig.maxMessageSize - maxKafkaKeySize );

    InputTopicConfiguration topicConfig( topicName );

    auto kafkaPublisher = new KafkaPublisher(pubConfig, topicConfig);
    kafkaPublisher->open();
    return kafkaPublisher;
}
