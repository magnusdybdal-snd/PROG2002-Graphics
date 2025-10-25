cmake_minimum_required(VERSION 3.15)

project(lab4)

find_package(OpenGL REQUIRED)

add_executable(lab4
main.cpp
Lab4Application.cpp
)

# Uncomment when adding the cube texture
# add_custom_command(
#     TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy
#     ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
#     ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.jpg
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.jpg)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")

target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_STATIC)

# glfw, glad and OpenGL::GL are linked trough GLFWApplication
target_link_libraries(lab4 
PRIVATE
Framework::GeometricTools
Framework::GLFWApplication
Framework::Rendering
stb
)