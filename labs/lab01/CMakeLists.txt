cmake_minimum_required(VERSION 3.15)

project(lab01)

find_package(OpenGL REQUIRED)

add_executable(HelloWorld main.cpp)   # Generate an executable named HelloWorld from main.cpp [7]

target_link_libraries(HelloWorld glfw glad OpenGL::GL)