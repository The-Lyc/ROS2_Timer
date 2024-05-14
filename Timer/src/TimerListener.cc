#include "../include/TimerListener.h"
#include <iostream>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

#include "TimerExecutor.h"

TimerListener::TimerListener(std::chrono::milliseconds interval, std::function<void()> callback)
: interval(interval), callback(callback), stop(false), wakeup_fd(eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC)) {
    if (wakeup_fd == -1) {
        std::cerr << "Failed to create eventfd" << std::strerror(errno) << std::endl;
    }
}

TimerListener::~TimerListener() {
    if(wakeup_fd != -1) {
        close(wakeup_fd);
    }
    stopListener(); // Make sure the thread is stopped before we exit
}

void TimerListener::start() {
    int wakeupFd = this->wakeup_fd;
    listener_thread = std::thread([this,wakeupFd] {

        TimerExecutor tExecutor(wakeupFd);
        std::thread executor_thread = std::thread([&tExecutor] {
            tExecutor.start();
        });

        while (!stop) {
            std::this_thread::sleep_for(interval);

            //TODO: 考虑submit回调列表和唤醒tExecutor线程这两件事的先后顺序，或者有其他方法
            
            std::cout << "TimerListener: wakeup!" << std::endl;
            if (!stop) tExecutor.submit(callback);
            
            wakeup(wakeup_fd);
        }
    });
}

void TimerListener::stopListener() {
    stop = true;
    if(listener_thread.joinable()) {
        wakeup(wakeup_fd); // Ensure we also wakeup the thread so it can exit
        listener_thread.join();
    }
}

void TimerListener::wakeup(int fd) {
    uint64_t one = 1;
    ssize_t n = write(fd, &one, sizeof(one));
    if (n != sizeof(one)) {
        std::cerr << "TimerListener: write error" << std::strerror(errno) << std::endl;
    }
}