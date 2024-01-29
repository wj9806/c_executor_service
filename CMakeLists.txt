cmake_minimum_required(VERSION 3.22)
project(c_executor_service)

set(CMAKE_CXX_STANDARD 14)

add_executable(c_executor_service
        main.c
        thread_pool.h
        thread_pool.c)
