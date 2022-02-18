#pragma once

#include "./exampleModel.h"
#include "../utils/myThread.h"

#include "transport/codec/binaryCodec.h"


class ReadMessage /* : public MyThread */
{
    static const Quix::Transport::BinaryCodec<ExampleModel> codec1;

    void registerCodecs();

public:
    void start();
};