cmake_minimum_required(VERSION 3.6)
project(clion)

include_directories(include)

set(SOURCE_FILES main.cpp)
add_executable(clion ${SOURCE_FILES})

add_executable(l6q1 src/L6/1/q.c)
add_executable(l6q2 src/L6/2/q.c)
add_executable(l6q3 src/L6/3/q.c)
add_executable(l6q4 src/L6/4/q.c)

add_executable(l7q1 src/L7/1/q.c)

add_executable(l8q1 src/L8/1-new/q.c)