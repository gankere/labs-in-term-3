cmake_minimum_required(VERSION 3.10.0)

# Версионирование
if(NOT DEFINED PROJECT_VERSION_MAJOR)
  set(PROJECT_VERSION_MAJOR 0)
endif()
if(NOT DEFINED PROJECT_VERSION_MINOR)
  set(PROJECT_VERSION_MINOR 0)
endif()
if(NOT DEFINED PATCH_VERSION)
  set(PATCH_VERSION 0)
endif()

set(PROJECT_VERSION_PATCH ${PATCH_VERSION})

project(lab2 VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH} LANGUAGES C CXX)

# Основная библиотека
add_library(functions STATIC functions.cpp)
target_include_directories(functions PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
add_library(lab2::functions ALIAS functions)

# Исполняемый файл
add_executable(lab2 main.cpp)
target_link_libraries(lab2 PRIVATE functions)

# Путь к файлу данных
target_compile_definitions(lab2 PRIVATE 
    IP_FILTER_PATH="${CMAKE_SOURCE_DIR}/ip_filter.tsv"
)

# Тесты
add_subdirectory(gtest_tests)  # ← ИСПРАВЛЕНО: убрал лишний путь

# CPack — упаковка в .zip / .deb
set(CPACK_PACKAGE_NAME "Lab2")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

if(WIN32)
  set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
  set(CPACK_GENERATOR "DEB")
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "avv100 <avv100@tpu.ru>")
endif()

install(TARGETS lab2 DESTINATION bin)

include(CPack)