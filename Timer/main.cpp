#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <fstream>
#include <unistd.h>

#include <muduo/net/Channel.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <sys/eventfd.h>

#include "sys/eventfd.h"
#include "SingleThreadExecutor.h"
#include "TimerExecutor.h"
#include "TimerListener.h"

using namespace muduo;
using namespace muduo::net;

class Task {
public:
    Task(const std::string& name) : name(name) {}

    void execute() {
        std::cout << "Executing task: " << name << std::endl;
    }

private:
    std::string name;
};


int main() {
    SingleThreadExecutor executor; // 创建一个单线程执行器

    // 提交任务到执行器
    // for (int i = 0; i < 10; ++i) {
    //     std::string task_name = "Task " + std::to_string(i);
    //     executor.submit([task_name] {
    //         Task task(task_name);
    //         task.execute();
    //     });
    // }

    // 创建定时器监听器
    TimerListener timerListener(std::chrono::milliseconds(3000), [] {
        std::cout << "Timer is ready!" << std::endl;
        std::ofstream out("out.txt");
        for(int i=0;i<1e6;i++){
        out<<i<<std::endl;
  }
  out.close();
    });
    timerListener.start();

    std::this_thread::sleep_for(std::chrono::milliseconds(100000));

    // 关闭执行器
    executor.shutdown();
    // 停止定时器监听器
    timerListener.stopListener();

    return 0;
}
