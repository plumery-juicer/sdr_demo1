# 必须放在最顶部
cmake_minimum_required(VERSION 3.10)

project(sdr_project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 只查找底层 zeromq 库
include_directories(include)

# ---------------- 源码列表 ----------------
set(SRC_SOURCES
    src/main.cpp
)

add_executable(sdr_app ${SRC_SOURCES})

target_include_directories(sdr_app
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        # cppzmq 头文件目录，Ubuntu默认安装在这里
        /usr/include
)

target_link_libraries(sdr_app PRIVATE zmq)