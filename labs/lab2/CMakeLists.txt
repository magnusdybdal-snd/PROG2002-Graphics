cmake_minimum_required(VERSION 3.15)

project(lab2)

find_package(OpenGL REQUIRED)

add_executable(lab2
main.cpp
Lab2Application.cpp
)  

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(lab02 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
)