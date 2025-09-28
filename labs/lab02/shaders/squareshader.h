#ifndef __SQUARESHADER_H_
#define __SQUARESHADER_H_

#include <string>

static const std::string squareVertexShaderSrc = R"(
#version 430 core

layout(location = 0) in vec3 a_Position;

void main()
{
gl_Position = vec4(a_Position, 1.0f);
}
)";

// Fragment shader that fills the triangle with color
const std::string squareFragmentShaderSrc = R"(
#version 430 core

out vec4 color;

uniform vec4 u_Color;

void main()
{
color = u_Color;
}
)";


#endif