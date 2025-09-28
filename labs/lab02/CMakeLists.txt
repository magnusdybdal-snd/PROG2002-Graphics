cmake_minimum_required(VERSION 3.15)

project(lab01)

add_executable(lab02 main.cpp Lab2Application.cpp)

target_link_libraries(lab02
    PRIVATE
    framework::GLFWApplication
)