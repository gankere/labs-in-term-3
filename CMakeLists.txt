cmake_minimum_required(VERSION 3.10) # 1. Минимальная версия CMake

# 2. Управление версией (адаптировано из предыдущего ответа и lab2)
# Проверяем, установлены ли переменные версии извне (например, из CI/CD)
if(NOT DEFINED PROJECT_VERSION_MAJOR)
  set(PROJECT_VERSION_MAJOR 0)
endif()
if(NOT DEFINED PROJECT_VERSION_MINOR)
  set(PROJECT_VERSION_MINOR 0)
endif()
if(NOT DEFINED PATCH_VERSION) # Переменная из CI/CD скрипта
  set(PATCH_VERSION 0)
endif()

# Устанавливаем PATCH в PROJECT_VERSION_PATCH для CMake
set(PROJECT_VERSION_PATCH ${PATCH_VERSION})

# 3. Объявление проекта с версией
project(lab3 VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

# 4. Установка стандарта C++
set(CMAKE_CXX_STANDARD 17) # Или 20, т.к. используется std::make_unique
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 5. Создание исполняемого файла
add_executable(lab3 main.cpp)

# 6. (Опционально) Передача версии в код
target_compile_definitions(lab3 PRIVATE
    PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
    PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
    PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
)

# 7. (Опционально, если используется CPack) Настройки пакетирования
# Эти строки можно перенести, если вы хотите использовать CPack в CI/CD
set(CPACK_PACKAGE_NAME "Lab3") # Измените имя
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_BINARY_DIR}/package")

if(WIN32)
  set(CPACK_GENERATOR "ZIP")
elseif(UNIX)
  set(CPACK_GENERATOR "DEB")
  # Убедитесь, что maintainer определён, если используете DEB
  set(CPACK_DEBIAN_PACKAGE_MAINTAINER "your_email@example.com") # Замените на ваш email
endif()

install(TARGETS lab3 DESTINATION bin)
include(CPack) # Включаем CPack в конец файла