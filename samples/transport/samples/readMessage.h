#pragma once

#include <string>

#include "./exampleModel.h"
#include "../utils/myThread.h"

#include "transport.kafka/kafkaSubscriber.h"

#include "transport/codec/binaryCodec.h"

#include "./const.h"


class ReadMessage /* : public MyThread */
{
    static const Quix::Transport::BinaryCodec<ExampleModel> codec1;

    const std::string inputGroup;
    const std::string topicName;

    std::mutex lock_;
    long long consumerCounter = 0;

    void registerCodecs();

    void newMessageHandler(std::shared_ptr<Quix::Transport::IPackage> package);

public:
    ReadMessage();
    Quix::Transport::ISubscriber* start( bool useConsumerGroup = true, Quix::Transport::Kafka::Offset offset = Quix::Transport::Kafka::Offset::Unset );
};