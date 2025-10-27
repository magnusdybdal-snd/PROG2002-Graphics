#ifndef __CHESSBOARD_VERTEX_H_
#define __CHESSBOARD_VERTEX_H_

#include <string>

static const std::string chessboardVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec2 a_Position;    // INPUT: 2D position from VBO

// Uniforms: transformation matrices
uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_chessboardModelMatrix;

out vec2 v_GridPos;                         // OUTPUT: Pass grid position to fragment shader

void main() 
{
    // Set vertex position in clip space.
    gl_Position = u_projectionMatrix * u_viewMatrix * u_chessboardModelMatrix * vec4(a_Position, 0.0, 1.0);

    // Convert from (-0.5, 0.5) to (0.0, 1.0) and pass to fragment shader
    v_GridPos = a_Position + 0.5;
}
)";

#endif 