#include "gtest/gtest.h"
#include "utils/timer.h"

#include <algorithm>
#include <mutex>
#include <chrono>         // std::chrono::seconds
#include <thread>         // std::this_thread::sleep_for


using namespace std;
using namespace Quix;


class TimerCounter : public Timer {
    int counter_ = 0;
    std::mutex mutex_;
public:
    TimerCounter(): Timer(){};
    TimerCounter(int delay): Timer(delay){};
    TimerCounter(int delay, int interval): Timer(delay, interval){};

    virtual void callback(){
        std::lock_guard<std::mutex> guard(mutex_);
        counter_++;
    }
    int counter(){
        std::lock_guard<std::mutex> guard(mutex_);
        return counter_;
    }
};



TEST(timerTest, BasicTimeout)
{
    //after 500ms do nothing

    TimerCounter mytimer;

    EXPECT_EQ( mytimer.counter(), 0 );


    std::this_thread::sleep_for (std::chrono::milliseconds(500));

    EXPECT_EQ( mytimer.counter(), 0);
}

TEST(timerTest, BasicTimeoutChange)
{
    //set initially for 200ms

    TimerCounter mytimer(200);

    //reset after 100ms to huge number so it would not be called
    std::this_thread::sleep_for (std::chrono::milliseconds(100));

    mytimer.change(1000);

    std::this_thread::sleep_for (std::chrono::milliseconds(300));

    EXPECT_EQ( mytimer.counter(), 0 );
}

TEST(timerTest, PrioritizeTimeout)
{
    //timeout for 200ms and then periodically tick after 50ms
    TimerCounter mytimer(200, 50);

    //reset after 100ms to huge number so it would not be called
    std::this_thread::sleep_for (std::chrono::milliseconds(100));

    //after 100ms >> 0 ticks
    EXPECT_EQ( mytimer.counter(), 0 );


    std::this_thread::sleep_for (std::chrono::milliseconds(330));

    //after 430ms >> at least 5 ticks ( 1x 200ms timeout + 4 x 50ms interval )
    EXPECT_GE( mytimer.counter(), 5 );
}



TEST(timerTest, BasicInterval)
{
    //after 200ms timeout increment counter

    TimerCounter mytimer(300);


    //after 100ms should not be still incrementing nothing
    std::thread thread([&](){
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        EXPECT_EQ( mytimer.counter(), 0 );
    });
    thread.join();

    //wait for 500ms
    std::this_thread::sleep_for (std::chrono::milliseconds(600));

    //should tick exactly once
    EXPECT_EQ( mytimer.counter(), 1 );
}

TEST(timerTest, NotCallOnDestroy)
{
    //after 200ms timeout increment counter

    int called = 0;
    {
        CallbackTimer mytimer([&](){called++;}, 200);

        //wait for 100ms but then destroy
        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        //here is the destructor for CallbackTimer called so the timer thead should be properly ended
    }

    EXPECT_EQ( called, 0 );
}


TEST(timerTest, TimeoutViaChange)
{
    //in 200ms interval increment counter
    TimerCounter mytimer;

    EXPECT_EQ( mytimer.counter(), 0 );

    //wait for 500ms
    std::this_thread::sleep_for (std::chrono::milliseconds(600));

    EXPECT_EQ( mytimer.counter(), 0 );


    mytimer.change( 200 );

    //wait for another 600ms
    std::this_thread::sleep_for (std::chrono::milliseconds(600));

    EXPECT_EQ( mytimer.counter(), 1 );
}


TEST(callbackTimerTest, simpleCallback)
{
    //in 200ms interval increment counter
    int counter = 0;

    CallbackTimer mytimer([&](){ counter++; }, 100);

    EXPECT_EQ( counter, 0 );

    //wait for 220ms
    std::this_thread::sleep_for (std::chrono::milliseconds(220));

    EXPECT_EQ( counter, 1 );
}

TEST(callbackTimerTest, changeCallback)
{
    //in 200ms interval increment counter
    int counter1 = 0;
    int counter2 = 0;

    CallbackTimer mytimer([&](){ counter1++; }, 100);
    mytimer.setAction([&](){ counter2++;});

    EXPECT_EQ( counter1, 0 );
    EXPECT_EQ( counter2, 0 );

    //wait for 220ms
    std::this_thread::sleep_for (std::chrono::milliseconds(220));

    EXPECT_EQ( counter1, 0 );
    EXPECT_EQ( counter2, 1 );
}
