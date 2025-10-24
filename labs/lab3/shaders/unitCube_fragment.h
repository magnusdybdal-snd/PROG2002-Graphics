#ifndef __UNITCUBE_FRAGMENT_H_
#define __UNITCUBE_FRAGMENT_H_

#include <string>

const std::string unitCubeFragmentShaderSrc = R"(
#version 430 core

uniform vec4 u_Color;           // Color of wireframe

out vec4 fragColor;             // OUTPUT: Color

void main()
{
    fragColor = u_Color;
}
)";

#endif