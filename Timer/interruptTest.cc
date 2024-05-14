#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThread.h"
#include "muduo/base/Thread.h"

#include "./include/SingleThreadExecutor.h"

#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

using namespace muduo;
using namespace muduo::net;

int cnt = 0;
EventLoop* g_loop;

void printTid()
{
  printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
  printf("now %s\n", Timestamp::now().toString().c_str());
}

void print(const char* msg)
{
  printf("msg %s %s\n", Timestamp::now().toString().c_str(), msg);
  if (++cnt == 20)
  {
    g_loop->quit();
  }
}

void cancel(TimerId timer)
{
  g_loop->cancel(timer);
  printf("cancelled at %s\n", Timestamp::now().toString().c_str());
}

void func(){
  std::ofstream out("out.txt");
  for(int i=0;i<1e6;i++){
    out<<i<<std::endl;
  }
  out.close();
  return;
}

int main()
{
  printTid();
  
  {
    EventLoop loop;
    g_loop = &loop;

    
    TimerId t3 = loop.runEvery(3, std::bind(func));


    loop.loop();

  }

}
