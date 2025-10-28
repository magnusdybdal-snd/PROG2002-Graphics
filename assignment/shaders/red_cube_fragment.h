#ifndef RED_CUBE_FRAGMENT_H_
#define RED_CUBE_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_RED_CUBE_FRAGMENT_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_RED_CUBE_FRAGMENT_VERSION = "#version 430 core\n";
#endif

inline std::string redCubeFragmentShaderSrc = std::string(GLSL_RED_CUBE_FRAGMENT_VERSION) + R"(

in vec3 vs_Position;            // INPUT: Position from vertex shader
uniform vec3 u_Color;
uniform int u_Selected;
out vec4 fragColor;             // OUTPUT: Color

void main()
{
   if(u_Selected == 1) { 
        fragColor = vec4(1.0, 1.0, 0.4, 1.0);
    } else {
        fragColor = vec4(u_Color, 1.0);
   }
}
)";

#endif