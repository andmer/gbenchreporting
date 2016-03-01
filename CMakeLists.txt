cmake_minimum_required(VERSION 3.3)
project(mfifo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(BOOST_ROOT "../boost/boost_1_60_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost REQUIRED system)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(../gbench/benchmark-1.0.0 gbench)
include_directories(../gbench/benchmark-1.0.0/include)

add_executable(bm bm.cpp)
target_link_libraries(bm
        benchmark
        ${Boost_LIBRARIES})

