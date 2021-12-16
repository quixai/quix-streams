#pragma once

#include <climits>

#include <atomic>

#include <mutex>
#include <vector>
#include <chrono>
#include <functional>
#include <condition_variable>
#include <thread>

#include <algorithm>


namespace Quix { 


/****
 * Wrapper over the std::function<void(_ArgTypes...)>
 *   allows handle event based 
 */

class Timer {

    /// function to be executed with tick
    // std::function<void()> cbk_ = nullptr;

    std::thread thread_;
    std::condition_variable cond_;

    std::atomic<bool> threadShouldBeRunning_;


    std::chrono::time_point<std::chrono::system_clock> lastRun_;

    std::mutex changePropsLock_;
    int delay_ = INFINITY;
    int interval_ = INFINITY;

    void run();

public:
    static const int INFINITY = INT_MAX;

    void change(int delay = INFINITY, int interval = INFINITY);

    void stop();

    // Timer(std::function<void()> cbk, int delay = INFINITY, int interval = INFINITY);
    Timer(int delay = INFINITY, int interval = INFINITY);

    virtual void callback() = 0;

    ~Timer();

};


class CallbackTimer : public Timer{
    std::function<void()> cbk_;
    public:
        CallbackTimer(std::function<void()> cbk_, int delay = INFINITY, int interval = INFINITY);
        void callback();
};


}