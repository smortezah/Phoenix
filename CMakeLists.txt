cmake_minimum_required(VERSION 2.8.12)
project(phoenix)

SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")

file(GLOB SOURCE_FILES "src/*.h" "src/*.cpp")
add_executable(phoenix ${SOURCE_FILES})