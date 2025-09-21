#ifndef __TRIANGLESHADER_H_
#define __TRIANGLESHADER_H_

#include <string>

// Vertex shader that forwards the coordinates
const std::string triangleVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color1;
layout(location = 2) in vec3 color2;

uniform uint u_AlternateFlag;

out vec4 v_Color;

void main()
{
gl_Position = vec4(position, 1.0); // Homogeneous coordinates 3D + 1

if(u_AlternateFlag != 1) {
    v_Color = vec4(color1, 1.0f); // Use color 1
} else {
    v_Color = vec4(color2, 1.0f); // Use color 2
}
}
)";

// Fragment shader that fills the triangle with color
const std::string triangleFragmentShaderSrc = R"(
#version 430 core

in vec4 v_Color;
out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = v_Color;
}
)";

#endif