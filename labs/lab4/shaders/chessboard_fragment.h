#ifndef __CHESSBOARD_FRAGMENT_H_
#define __CHESSBOARD_FRAGMENT_H_

#include <string>

const std::string chessboardFragmentShaderSrc = R"(
#version 430 core

in vec2 v_GridPos;                                              // INPUT:  Interpolated from vertex shader (0 to 1)
in vec2 v_Tcoords;                                              // INPUT:  tcoords from vertex shader
uniform ivec2 u_SelectedTile;                                   // INPUT:  From C++ code (which tile is selected)
uniform int u_Gridsize;                                         // INPUT:  Board grid size set in c++ code
layout(binding = 0) uniform sampler2D u_floorTextureSampler;    // INPUT:  Floor texture sampler
out vec4 fragColor;                                             // OUTPUT: Final pixel color

void main()
{

    // DEBUG: Check if uniform is being passed
    if (u_Gridsize == 0) {
        fragColor = vec4(1.0, 0.0, 1.0, 1.0); // Bright magenta if broken
        return;
    }

    // Convert continous position (0, 1) to tile coordinates (0, 7)
    int tileX = int(floor(v_GridPos.x * float(u_Gridsize)));
    int tileY = int(floor(v_GridPos.y * float(u_Gridsize)));

    // Clamp to valid range for safety
    tileX = clamp(tileX, 0, u_Gridsize - 1);
    tileY = clamp(tileY, 0, u_Gridsize - 1);

    // Check if this is the selected tile
    bool isSelected = (tileX == u_SelectedTile.x && tileY == u_SelectedTile.y);

    // Sample the texture color
    vec4 textureColor = texture(u_floorTextureSampler, v_Tcoords);
    vec4 chessboardColor;

    // Checkerboard pattern
    bool isBlack = ((tileX + tileY) % 2) == 0;

    // Set color based on tile type
    if (isSelected) {
        // Selected tile: Green
        fragColor = vec4(0.0, 0.9, 0.0, 1.0) * 0.7 + textureColor * 0.3;
    } else {

        if (isBlack) {
            // Black tile
            chessboardColor = vec4(0.1, 0.1, 0.1, 1.0);
        }
        else if (!isBlack) {
            // White color
            chessboardColor = vec4(0.9, 0.9, 0.9, 1.0);
        }

        fragColor = mix(chessboardColor, textureColor, 0.7);    
    }
}
)";

#endif // CHESSBOARD_FRAGMENT_H_