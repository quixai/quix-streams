#pragma once

#include <thread>

class MyThread
{
    std::thread thread_;

    void wrappedRun();
protected:
    bool running_ = false;
    virtual void run() = 0;
public:
    void start();
    void join();
    
};
