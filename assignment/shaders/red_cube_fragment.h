#ifndef RED_CUBE_FRAGMENT_H_
#define RED_CUBE_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_RED_CUBE_FRAGMENT_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_RED_CUBE_FRAGMENT_VERSION = "#version 430 core\n";
#endif

inline std::string redCubeFragmentShaderSrc = std::string(GLSL_RED_CUBE_FRAGMENT_VERSION) + R"(
uniform samplerCube u_CubeSampler;

in vec3 vs_Position;            // INPUT: Position from vertex shader
uniform vec3 u_Color;
uniform int u_Selected;
uniform int u_TextureEnabled;
out vec4 fragColor;             // OUTPUT: Color

void main()
{
    vec4 cubeColor;
    vec4 textureColor = texture(u_CubeSampler, vs_Position);

    if (u_Selected == 1) { 
        cubeColor = vec4(1.0, 1.0, 0.4, 1.0);
    } else {
        cubeColor = vec4(u_Color, 1.0);
   }
   if (u_TextureEnabled == 0){
        fragColor = cubeColor;
    } else{
        fragColor = mix(cubeColor, textureColor, 0.4);
    }
}
)";

#endif