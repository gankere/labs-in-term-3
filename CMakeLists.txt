cmake_minimum_required(VERSION 3.20)
project(lab6)
set(CMAKE_CXX_STANDARD 17)
add_executable(lab6
    main.cpp
    Controller/Controller.cpp
    View/View.cpp
    Model/Document.cpp
)