#ifndef CHESS_ASSIGNMENT_VERTEX_H_
#define CHESS_ASSIGNMENT_VERTEX_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_VERSION = "#version 430 core\n";
#endif

inline std::string chessboardVertexShaderSrc = std::string(GLSL_VERSION) + R"(

layout(location = 0) in vec2 a_Position;    // INPUT: 2D position from VBO

// Uniforms: transformation matrices
uniform mat4 u_chessboardModelMatrix;

out vec2 v_GridPos;                         // OUTPUT: Pass grid position to fragment shader

void main() 
{
    // Set vertex position in clip space.
    gl_Position = u_chessboardModelMatrix * vec4(a_Position, 0.0, 1.0);

    // Convert from (-0.5, 0.5) to (0.0, 1.0) and pass to fragment shader
    v_GridPos = a_Position + 0.5;

}
)";

#endif //CHESS_ASSIGNMENT_VERTEX_H_