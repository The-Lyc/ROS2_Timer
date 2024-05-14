cmake_minimum_required(VERSION 3.0)
project(Timer)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)

# 添加头文件路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 添加源文件
set(SOURCE_FILES
    main.cpp
    src/SingleThreadExecutor.cc
    src/TimerListener.cc
    src/TimerExecutor.cc
)

# 指定生成目标
add_executable(timer ${SOURCE_FILES})

target_link_libraries(timer muduo_net muduo_base)
