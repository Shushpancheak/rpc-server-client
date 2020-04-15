cmake_minimum_required(VERSION 3.15)
project(server-client)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads)

include_directories(include)

add_executable(server src/daemon-main.cpp
        include/daemon.hpp
        src/daemon.cpp)
add_executable(client src/client-main.cpp)

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})