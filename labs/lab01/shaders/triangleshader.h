#ifndef __TRIANGLESHADER_H_
#define __TRIANGLESHADER_H_

#include <string>

// Vertex shader that forwards the coordinates
const std::string triangleVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 position;

void main()
{
gl_Position = vec4(position, 1.0); // Homogeneous coordinates 3D + 1
}
)";

// Fragment shader that fills the triangle with color
const std::string triangleFragmentShaderSrc = R"(
#version 430 core

out vec4 color;
void main()
{
color = vec4(0.0, 0.0, 1.0, 1.0);
}
)";

#endif