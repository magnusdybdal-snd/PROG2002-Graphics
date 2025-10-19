cmake_minimum_required(VERSION 3.15)

project(lab01)

find_package(OpenGL REQUIRED)

add_executable(lab01 main.cpp)   # Generate an executable named lab01 from main.cpp [7]

target_link_libraries(lab01 glfw glad OpenGL::GL Framework::GeometricTools)