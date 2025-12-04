cmake_minimum_required(VERSION 3.10.0)
project(lab6)
set(CMAKE_CXX_STANDARD 17)

# Управление версией через CI/CD
if(NOT DEFINED PROJECT_VERSION_MAJOR)
    set(PROJECT_VERSION_MAJOR 0)
endif()
if(NOT DEFINED PROJECT_VERSION_MINOR)
    set(PROJECT_VERSION_MINOR 1)
endif()
if(NOT DEFINED PATCH_VERSION)
    set(PATCH_VERSION 0)
endif()

project(lab6 VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PATCH_VERSION} LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(lab6
    main.cpp
    Controller/Controller.cpp
    View/View.cpp
    Model/Document.cpp
)

target_compile_definitions(lab6 PRIVATE
    VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    VERSION_MINOR=${PROJECT_VERSION_MINOR}
    VERSION_PATCH=${PATCH_VERSION}
)

if(WIN32)
    target_compile_definitions(lab6 PRIVATE _CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Опции компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(lab6 PRIVATE -Wall -Wextra)
endif()

# Информация о сборке
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")