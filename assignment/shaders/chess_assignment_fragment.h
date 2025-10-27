#ifndef CHESSASSIGNMENT_FRAGMENT_H_
#define CHESSASSIGNMENT_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_VERSION = "#version 430 core\n";
#endif

inline std::string chessboardFragmentShaderSrc = std::string(GLSL_VERSION) + R"(

in vec2 v_GridPos;                                              // INPUT:  Interpolated from vertex shader (0 to 1)
uniform int u_Gridsize;                                         // INPUT:  Board grid size set in c++ code

out vec4 fragColor;                                             // OUTPUT: Final pixel color

void main()
{
    vec4 chessboardColor;

    // Checkerboard pattern
    bool isBlack = ((tileX + tileY) % 2) == 0;

    // Set color based on tile type

    if (isBlack) {
        // Black tile
        chessboardColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
    else if (!isBlack) {
        // White color
        chessboardColor = vec4(0.9, 0.9, 0.9, 1.0);
    }

        fragColor = chessboardColor;    
}
)";

#endif // CHESSBOARD_FRAGMENT_H_