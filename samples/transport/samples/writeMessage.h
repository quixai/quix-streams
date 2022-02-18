#pragma once

#include "transport/io/IPublisher.h"
#include "transport.kafka/IKafkaPublisher.h"
#include "transport.kafka/kafkaPublisher.h"
#include "transport/fw/byteSplitter.h"

#include "../utils/myThread.h"
#include "./const.h"


class WriteMessage : public MyThread
{
protected:
    int maxMessageSizeInKafka = 1010;
    bool doWork_ = true;

    const std::string topicName = Const::MessagesTopic;
    // public int MaxMessageSizeInKafka = 1010;
    int maxKafkaKeySize = 100;
    int messageSizeInBytes = 2048;
    int millisecondsInterval = 1000; // interval between messages sent 0 = none

    void run();

    void sendMessage( Quix::Transport::IPublisher* publisher );
    Quix::Transport::Kafka::IKafkaPublisher* createPublisher(Quix::Transport::ByteSplitter& byteSplitter);

};