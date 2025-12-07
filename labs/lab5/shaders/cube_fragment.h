#ifndef CUBE_FRAGMENT_H_
#define CUBE_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_CUBE_FRAGMENT_VERSION = "#version 410 core\nuniform samplerCube u_CubeSampler;";
#else
    constexpr const char* GLSL_CUBE_FRAGMENT_VERSION = "#version 430 core\nlayout(binding = 1) uniform samplerCube u_CubeSampler;";
#endif

inline std::string cubeFragmentShaderSrc = std::string(GLSL_CUBE_FRAGMENT_VERSION) + R"(

in vec4 vs_FragPosition;        // INPUT: Position from vertex shader (world space)
in vec4 vs_Normal;              // INPUT: Normal position from vertex shader

uniform vec3 u_Color;
uniform int u_TextureEnabled;
uniform float u_ambientStrength = 1.0;
uniform vec3 u_lightSourcePosition;
uniform float u_diffuseStr;

out vec4 fragColor;             // OUTPUT: Color

void main()
{
    vec4 textureColor = texture(u_CubeSampler, vs_FragPosition.xyz);

    vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_FragPosition.xyz));
    float diffuseStrength = max(dot(lightDirection, vs_Normal.xyz), 0.0) * u_diffuseStr;

   if (u_TextureEnabled == 0){
        fragColor = vec4(u_Color, 1.0) * (u_ambientStrength + diffuseStrength);
    } else {
        fragColor = mix(vec4(u_Color, 1.0), textureColor, 0.4) * (u_ambientStrength + diffuseStrength);
    }
}
)";

#endif