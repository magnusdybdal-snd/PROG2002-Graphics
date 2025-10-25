cmake_minimum_required(VERSION 3.15)

project(lab4)

find_package(OpenGL REQUIRED)

add_executable(lab4
main.cpp
Lab4Application.cpp
)  

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(lab4 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
Framework::Rendering
)