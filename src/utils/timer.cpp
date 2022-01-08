#include "./timer.h"

#include <iostream>

namespace Quix { 

    int Timer::calculateNextWaitTime() const
    {
            int delay = delay_;
            int interval = interval_;

            int waitFor;
            if( delay != INFINITE )
            {
                waitFor = delay;
            }
            else if( interval != INFINITE )
            {
                waitFor = interval;
            }
            else
            {
                waitFor = INFINITE;
            }
            return waitFor;
    }

    std::chrono::duration<double, std::nano> Timer::calculateNextWait(int waitFor) const
    {
            //// load configuration for the waiting
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            auto elapsed = now - lastRun_;  // in miliseconds

            /// TODO: implement proper thread wake up
            auto toWait = std::chrono::duration<double, std::milli>(waitFor) - elapsed;        

            return toWait;
    }


    void Timer::run()
    {
        threadIsRunning_ = true;
        do
        {
            std::unique_lock<std::mutex> lk(changePropsLock_);

            int waitFor = calculateNextWaitTime();           
            auto toWait = calculateNextWait(waitFor);

            if( toWait > std::chrono::duration<double, std::milli>(0) )
            {
                cond_.wait_for(lk, toWait, [=](){ 
                    int newWaitFor = calculateNextWaitTime();
                    return 
                        !this->threadShouldBeRunning_ 
                            ||
                        waitFor != newWaitFor
                            || 
                        this->calculateNextWait(newWaitFor) <= std::chrono::duration<double, std::milli>(0); 
                    });
            }

            //// thread could have been stopped
            if(!threadShouldBeRunning_)
            {
                return;
            }


            //// load again configuration for waiting becaue condition can expire due to other reasons than timeout
            int delay = delay_;
            int interval = interval_;
            waitFor;

            if( delay != INFINITE )
            {
                waitFor = delay;
            }
            else if( waitFor != INFINITE )
            {
                waitFor = interval;
            }
            else
            {
                waitFor = INFINITE;
            }

            //// check elapsed time
            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            auto elapsed = now - lastRun_;
            if( elapsed >= std::chrono::duration<double, std::milli>(waitFor) )
            {
                // if it is only timeout then reset timer for timeout
                if( delay != INFINITE )
                {
                    delay_ = INFINITE;
                }

                //// execute time
                callback();



                lastRun_ = std::chrono::system_clock::now();
            }


            /* code */
        } while ( threadShouldBeRunning_ );

        threadIsRunning_ = false;
        
    }


    void Timer::change(int delay, int interval)
    {
        std::lock_guard<std::mutex> guard(changePropsLock_);

        lastRun_ = std::chrono::system_clock::now();

        delay_ = delay;
        interval_ = interval;

        if( !threadShouldBeRunning_ )
        {
            threadShouldBeRunning_ = true;
            thread_ = std::thread(&Timer::run, this);

        }
        cond_.notify_all();
    }

    void Timer::stop()
    {
        std::lock_guard<std::mutex> guard(changePropsLock_);

        lastRun_ = std::chrono::system_clock::now();

        delay_ = INFINITE;
        interval_ = INFINITE;

        cond_.notify_all();
    }


    Timer::Timer(int delay, int interval)
    :
    threadShouldBeRunning_(false),
    threadIsRunning_(false)
    {
        change(delay, interval);
    }

    Timer::~Timer()
    {
        threadShouldBeRunning_ = false;
        stop();

        thread_.join();
    }


    CallbackTimer::CallbackTimer(std::function<void()> cbk, int delay, int interval)
    :
    cbk_(cbk),
    Timer(delay, interval)
    {

    }

    void CallbackTimer::callback()
    {
        cbk_();
    }

    void CallbackTimer::setAction(std::function<void()> cbk)
    {
        cbk_ = cbk;
    }

}