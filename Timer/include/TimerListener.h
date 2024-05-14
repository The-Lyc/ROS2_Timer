#ifndef TIMER_LISTENER_H
#define TIMER_LISTENER_H

#include <chrono>
#include <functional>
#include <thread>

class TimerListener {
public:
    TimerListener(std::chrono::milliseconds interval, std::function<void()> callback); 
    ~TimerListener();

    void start();
    void stopListener();
    void wakeup(int wakeup_fd);

private:
    std::chrono::milliseconds interval;
    std::function<void()> callback;
    std::thread listener_thread;
    bool stop;
    int wakeup_fd;
};

#endif // TIMER_LISTENER_H