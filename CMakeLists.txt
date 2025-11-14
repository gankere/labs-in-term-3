cmake_minimum_required(VERSION 3.10)

# Управление версией через CI/CD
if(NOT DEFINED PROJECT_VERSION_MAJOR)
    set(PROJECT_VERSION_MAJOR 0)
endif()
if(NOT DEFINED PROJECT_VERSION_MINOR)
    set(PROJECT_VERSION_MINOR 0)
endif()
if(NOT DEFINED PATCH_VERSION)
    set(PATCH_VERSION 0)
endif()

project(lab3 VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PATCH_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основной исполняемый файл
add_executable(lab4 main.cpp)

# Передача версии в код
target_compile_definitions(lab4 PRIVATE
    VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    VERSION_MINOR=${PROJECT_VERSION_MINOR}
    VERSION_PATCH=${PATCH_VERSION}
)