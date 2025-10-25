#ifndef __UNITCUBE_FRAGMENT_H_
#define __UNITCUBE_FRAGMENT_H_

#include <string>

const std::string unitCubeFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_position;            // INPUT: Position from vertex shader

layout(binding = 1) uniform samplerCube u_cubeTexture;

uniform vec4 u_Color;           // Color of wireframe

out vec4 fragColor;             // OUTPUT: Color

void main()
{
    fragColor = texture(u_cubeTexture, vs_position);
}
)";

#endif