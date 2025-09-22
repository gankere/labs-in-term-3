cmake_minimum_required(VERSION 3.10.0)
project(lab2 VERSION 0.1.0 LANGUAGES C CXX)

add_library(functions STATIC functions.cpp)
target_include_directories(functions PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE functions)

target_compile_definitions(lab2 PRIVATE 
    IP_FILTER_PATH="${CMAKE_SOURCE_DIR}/ip_filter.tsv"
)