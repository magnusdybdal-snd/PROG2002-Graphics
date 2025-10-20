cmake_minimum_required(VERSION 3.15)

project(lab2_a)

find_package(OpenGL REQUIRED)

add_executable(lab2_a
main.cpp
Lab2_aApplication.cpp
)  

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(lab2_a 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
Framework::Rendering
)