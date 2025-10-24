#ifndef __UNITCUBE_FRAGMENT_H_
#define __UNITCUBE_FRAGMENT_H_

#include <string>

const std::string unitCubeFragmentShaderSrc = R"(
#version 430 core

out vec4 fragColor;             // OUTPUT: Color

void main()
{
    // Simple white color for now
    fragColor = vec4(0.9, 0.9, 0.9, 1.0);
}
)";

#endif