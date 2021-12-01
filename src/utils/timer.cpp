#include "./timer.h"

namespace Quix { namespace Transport {

    void Timer::run()
    {
        do
        {
            std::unique_lock<std::mutex> lk(changePropsLock_);
            
            //// load configuration for the waiting
            int delay = delay_;
            int interval = interval_;

            int waitFor;
            if( delay != INFINITY )
            {
                waitFor = delay;
            }
            else if( waitFor != INFINITY )
            {
                waitFor = interval;
            }
            else
            {
                waitFor = INFINITY;
            }

            std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
            auto elapsed = now - lastRun_;  // in miliseconds


            //// initialize wait

            //// TODO: can be waiting for negative time
            cond_.wait_for(lk, std::chrono::duration<double, std::milli>(waitFor) - elapsed);

            //// thread could have been stopped
            if(!threadShouldBeRunning_)
            {
                return;
            }


            //// load again configuration for waiting becaue condition can expire due to other reasons than timeout
            delay = delay_;
            interval = interval_;

            if( delay != INFINITY )
            {
                waitFor = delay;
            }
            else if( waitFor != INFINITY )
            {
                waitFor = interval;
            }
            else
            {
                waitFor = INFINITY;
            }

            //// check elapsed time
            now = std::chrono::system_clock::now();
            elapsed = now - lastRun_;
            if( elapsed >= std::chrono::duration<double, std::milli>(waitFor) )
            {
                //// execute time
                cbk_();

                lastRun_ = std::chrono::system_clock::now();
            }


            /* code */
        } while ( threadShouldBeRunning_ );
        
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
    }

    void Timer::stop()
    {
        std::lock_guard<std::mutex> guard(changePropsLock_);

        lastRun_ = std::chrono::system_clock::now();

        delay_ = INFINITY;
        interval_ = INFINITY;
    }


    Timer::Timer(std::function<void()> cbk, int delay, int interval)
    :
    threadShouldBeRunning_(false),
    cbk_(cbk)
    {
        change(delay, interval);
    }

    Timer::~Timer()
    {
        bool threadWasRunning = threadShouldBeRunning_;
        threadShouldBeRunning_ = false;
        stop();

        if ( threadWasRunning )
        {
            thread_.join();
        }
    }




} }