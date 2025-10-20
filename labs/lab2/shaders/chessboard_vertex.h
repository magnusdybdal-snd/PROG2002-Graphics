#ifndef __CHESSBOARD_VERTEX_H_
#define __CHESSBOARD_VERTEX_H_

#include <string>

static const std::string chessboardVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec2 a_Position; // 2D Pos from VBO

out vec2 v_GridPos; // Pass grid position to fragment shader

void main() 
{
    // Pass position trough to fragment shader
    gl_Position = vec4(a_Position, 0.0, 1.0);

    // Convert from (-0.5, 0.5) to (0.0, 1.0)
    v_GridPos = a_Position + 0.5;
}
)";

#endif 