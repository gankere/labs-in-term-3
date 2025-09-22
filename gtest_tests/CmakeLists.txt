cmake_minimum_required(VERSION 3.14)
project(GTestTests)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(GTestTests tests.cpp)
target_link_libraries(GTestTests PRIVATE functions gtest gtest_main)