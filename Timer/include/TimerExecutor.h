#ifndef TIMER_EXECUTOR_H
#define TIMER_EXECUTOR_H

#include "TimerListener.h" // 假设这是TimerListener类的头文件
#include "TimerListener.h"
#include "SingleThreadExecutor.h"

class TimerExecutor
{
private:
    SingleThreadExecutor *executor;
    int wakeup_fd;
public:
    TimerExecutor(int wakeup_fd);
    ~TimerExecutor();

    void start();

    template<class F>
    void submit(F&& f){
        this->executor->submit(f);
    };
};

#endif // TIMER_EXECUTOR_H