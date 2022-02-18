#include <iostream>
#include <thread>

#include "./myThread.h"

using namespace std;

void MyThread::wrappedRun()
{
    running_ = true;
    try
    {
        run();
    }
    catch(...)
    {
        cerr << "UNKNOWN ERROR IN THREAD RUN" << endl;
        // std::cerr << e.what() << '\n';
    }
    running_ = false;
}

void MyThread::start()
{
    thread_ = std::thread( &MyThread::run, this );
}

void MyThread::join()
{
    thread_.join();
}
