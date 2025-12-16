cmake_minimum_required(VERSION 3.10.0)
project(lab7 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS filesystem program_options system)

include_directories(
    ${CMAKE_SOURCE_DIR}/headers
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(lab7 ${SOURCES})

target_link_libraries(lab7
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${ZLIB_LIBRARIES}
)