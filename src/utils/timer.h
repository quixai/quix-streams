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

    std::atomic<bool> threadIsRunning_;
    std::atomic<bool> threadShouldBeRunning_;


    std::chrono::time_point<std::chrono::system_clock> lastRun_;

    std::mutex changePropsLock_;
    bool autoStart_;
    int delay_ = INFINITE;
    int interval_ = INFINITE;

    std::chrono::duration<double, std::nano> calculateNextWait(int waitFor) const;
    int calculateNextWaitTime() const;

    void run();

public:
    static const int INFINITE = INT_MAX;

    void change(int delay = INFINITE, int interval = INFINITE);

    void stop();
    void start();

    // Timer(std::function<void()> cbk, int delay = INFINITE, int interval = INFINITE);
    Timer(int delay = INFINITE, int interval = INFINITE, bool autoStart = true);

    virtual void callback() = 0;

    ~Timer();

};


class CallbackTimer : public Timer{
    std::function<void()> cbk_;
    public:
        CallbackTimer(std::function<void()> cbk_ = [](){}, int delay = INFINITE, int interval = INFINITE, bool autoStart = true);
        void setAction(std::function<void()> cbk);
        void callback();
};


}