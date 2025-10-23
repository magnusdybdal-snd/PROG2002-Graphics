cmake_minimum_required(VERSION 3.15)

project(lab3)

find_package(OpenGL REQUIRED)

add_executable(lab3
main.cpp
Lab3Application.cpp
)  

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(lab3 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
Framework::Rendering
)