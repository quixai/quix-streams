#pragma once

#include <string>

#include "./exampleModel.h"
#include "../utils/myThread.h"

#include "utils/timer.h"
#include "transport.kafka/kafkaSubscriber.h"

#include "transport/codec/binaryCodec.h"

#include "./const.h"


class ReadMessage /* : public MyThread */
{
    static const Quix::Transport::BinaryCodec<ExampleModel> codec1;

    const std::string inputGroup = "Test-Subscriber#1";
    const std::string topicName = Const::MessagesTopic;

    std::mutex lock_;
    long long subscribedCounter = 0;

    Quix::CallbackTimer* statTimer_ = nullptr;

    void registerCodecs();
    void hookupStatistics();

    void newMessageHandler(std::shared_ptr<Quix::Transport::IPackage> package);

public:
    ReadMessage();
    ~ReadMessage();
    Quix::Transport::ISubscriber* start( bool useConsumerGroup = true, Quix::Transport::Kafka::Offset offset = Quix::Transport::Kafka::Offset::Unset );
};