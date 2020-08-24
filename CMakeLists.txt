cmake_minimum_required(VERSION 3.17)
project(CircularBuffer)

find_package(Corrade REQUIRED Containers)

set(CMAKE_CXX_STANDARD 17)

add_executable(CircularBuffer main.cpp CircularBuffer.h)
target_link_libraries(CircularBuffer PRIVATE Corrade::Containers)