#ifndef SINGLE_THREAD_EXECUTOR_H
#define SINGLE_THREAD_EXECUTOR_H

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class SingleThreadExecutor {
public:
    SingleThreadExecutor();
    ~SingleThreadExecutor();

    void start();

    void start_once();

    template<class F>
    void submit(F&& f){
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    };

    void shutdown();

private:
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif // SINGLE_THREAD_EXECUTOR_H