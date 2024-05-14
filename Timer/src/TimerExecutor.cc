#include "../include/TimerExecutor.h"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/eventfd.h>
#include <cerrno>

#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include "include.h"

using namespace muduo;
using namespace muduo::net;


TimerExecutor::TimerExecutor(int wakeup_fd)
{
    this->wakeup_fd = wakeup_fd;
    SingleThreadExecutor sExecutor; 
    this->executor = &sExecutor;
}

TimerExecutor::~TimerExecutor()
{
    
}


void TimerExecutor::start(){
    // EventLoopThread loop_thread;  // 创建一个新的线程用于监听
    // EventLoop* loop = loop_thread.startLoop();
    EventLoop* loop;

    int wakeupFd = wakeup_fd;
    SingleThreadExecutor* sExecutor = this->executor; 
    Channel channel(loop, wakeup_fd);

    channel.setReadCallback([wakeupFd,sExecutor](muduo::Timestamp) {
        uint64_t val;
        ssize_t n = ::read(wakeupFd, &val, sizeof(val));
        if (n != sizeof(val)) {
            std::cerr << "TimerExecutor: read error" << std::endl;
        }
        std::cout << "TimerExecutor: wakeup event triggered" << std::endl;
        
        sExecutor->start_once();
    });
    
    channel.enableReading();

    loop->loop(); // 主线程进入事件循环
}

