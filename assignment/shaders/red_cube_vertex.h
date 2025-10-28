#ifndef RED_CUBE_VERTEX_H
#define RED_CUBE_VERTEX_H

#include <string>
#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_RED_CUBE_VERTEX_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_RED_CUBE_VERTEX_VERSION = "#version 430 core\n";
#endif

inline std::string redCubeVertexShaderSrc = std::string(GLSL_RED_CUBE_VERTEX_VERSION) + R"(
layout(location = 0) in vec3 a_Position;    // INPUT: 3D position from VBO

// Uniforms: transformation matrices
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_CubeModelMatrix;

out vec3 vs_Position;                       // OUTPUT: pass position to fragmentshader

void main() 
{
    // Set vertex position in clip space.
    gl_Position = u_ViewProjectionMatrix * u_CubeModelMatrix * vec4(a_Position, 1.0);

    vs_Position = a_Position;
}
)";

#endif 