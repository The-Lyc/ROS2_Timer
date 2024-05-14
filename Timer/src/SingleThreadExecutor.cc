#include "../include/SingleThreadExecutor.h"

SingleThreadExecutor::SingleThreadExecutor() : stop(false) {
    
}

SingleThreadExecutor::~SingleThreadExecutor() { 
    shutdown();

}

void SingleThreadExecutor::start() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop && tasks.empty()) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}

void SingleThreadExecutor::start_once() {
    while (true) {
        std::function<void()> task;
        {
            if(tasks.empty())   return;
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            if (stop) return;
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}


void SingleThreadExecutor::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_one();
}