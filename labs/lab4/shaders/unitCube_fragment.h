#ifndef __UNITCUBE_FRAGMENT_H_
#define __UNITCUBE_FRAGMENT_H_

#include <string>

const std::string unitCubeFragmentShaderSrc = R"(
#version 430 core

in vec3 vs_position;            // INPUT: Position from vertex shader

layout(binding = 1) uniform samplerCube u_cubeTexture;

uniform vec4 u_cubeBlendColor = vec4(0.0, 0.0, 0.0, 0.0);

uniform vec4 u_Color;           // Color of wireframe

out vec4 fragColor;             // OUTPUT: Color

void main()
{
    fragColor = mix(u_cubeBlendColor, texture(u_cubeTexture, vs_position), 0.7);
}
)";

#endif