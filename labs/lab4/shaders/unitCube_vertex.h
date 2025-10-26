#ifndef __UNITCUBE_VERTEX_H
#define __UNITCUBE_VERTEX_H

#include <string>

static const std::string unitCubeVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 a_Position;    // INPUT: 3D position from VBO

// Uniforms: transformation matrices
uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_unitCubeModelMatrix;

out vec3 vs_position;                       // OUTPUT: position for cubemap sampling

void main() 
{
    // Set vertex position in clip space.
    gl_Position = u_ViewProjectionMatrix * u_unitCubeModelMatrix * vec4(a_Position, 1.0);

    // Pass the position for cubemap position coordinates
    vs_position = a_Position;
}
)";

#endif 