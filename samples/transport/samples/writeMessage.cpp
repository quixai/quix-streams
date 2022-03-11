
#include "./writeMessage.h"

#include "../utils/ptrLock.h"

#include "transport.kafka/kafkaPublisher.h"
#include "transport.kafka/extensions/kafkaPackageExtensions.h"
#include "transport/transportPublisher.h"

#include <iostream>
#include <sstream>

#include <chrono>
#if _WIN32
  #include <windows.h>
#endif

#include <thread>

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

        std::shared_ptr<IPackage> package(new ByteArrayPackage( ByteArray::initRandom( this->messageSizeInBytes ) ));

        stringstream ss;
        int c;
        {
            std::lock_guard<std::mutex> lock(publishedCounterLock);
            c = counter;
        }
        ss << "CustomSize " << c << flush;
        KafkaPackageExtensions<IPackage>( package.get() ).setKey( ss.str() );

        try{
            publisher->send( package );

            {
                std::lock_guard<std::mutex> lock(publishedCounterLock);
                counter++;

                /// TODO: after async task, implement as well published counter
                publishedCounter++;
            }
        }catch(...)
        {
            cerr << "Error during send message" << endl;
        }

        if ( this->millisecondsInterval > 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(this->millisecondsInterval));
        }


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
