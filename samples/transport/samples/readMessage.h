#pragma once

#include "../utils/myThread.h"

class ReadMessage : public MyThread
{
protected:
    void run();
};