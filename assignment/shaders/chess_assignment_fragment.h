#ifndef CHESS_ASSIGNMENT_FRAGMENT_H_
#define CHESS_ASSIGNMENT_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_FRAGMENT_VERSION = "#version 410 core\n";
#else
    constexpr const char* GLSL_FRAGMENT_VERSION = "#version 430 core\n";
#endif

inline std::string chessboardFragmentShaderSrc = std::string(GLSL_FRAGMENT_VERSION) + R"(


in vec2 v_GridPos;                                              // INPUT:  Interpolated from vertex shader (0 to 1)
uniform int u_Gridsize;                                         // INPUT:  Board grid size set in c++ code

out vec4 fragColor;                                             // OUTPUT: Final pixel color

void main()
{
    vec4 chessboardColor;

    // Convert continuous position (0, 1) to tile coordinates (0, 7)
    // Clamp first to avoid edge cases where v_GridPos might be slightly outside [0,1]
    vec2 clampedPos = clamp(v_GridPos, 0.0, 0.9999);
    
    int tileX = int(clampedPos.x * float(u_Gridsize));
    int tileY = int(clampedPos.y * float(u_Gridsize));

    // Checkerboard pattern
    bool isBlack = ((tileX + tileY) % 2) == 0;

    // Set color based on tile type

    if (isBlack) {
        // Black tile
        chessboardColor = vec4(0.1, 0.1, 0.1, 1.0);
    }
    else {
        // White color
        chessboardColor = vec4(0.9, 0.9, 0.9, 1.0);
    }

        fragColor = chessboardColor;    
}
)";

#endif // CHESS_ASSIGNMENT_FRAGMENT_H_