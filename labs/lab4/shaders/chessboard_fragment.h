#ifndef __CHESSBOARD_FRAGMENT_H_
#define __CHESSBOARD_FRAGMENT_H_

#include <string>

const std::string chessboardFragmentShaderSrc = R"(
#version 430 core

in vec2 v_GridPos;                                              // INPUT:  Interpolated from vertex shader (0 to 1)
in vec2 v_Tcoords;                                              // INPUT:  tcoords from vertex shader
uniform ivec2 u_SelectedTile;                                   // INPUT:  From C++ code (which tile is selected)
layout(binding = 0) uniform sampler2D u_floorTextureSampler     // INPUT:  Floor texture sampler
out vec4 fragColor;                                             // OUTPUT: Final pixel color

void main()
{
    // Convert continous position (0, 1) to tile coordinates (0, 7)
    int tileX = int(floor(v_GridPos.x * 8.0));
    int tileY = int(floor(v_GridPos.y * 8.0));

    // Clamp to valid range for safety
    tileX = clamp(tileX, 0, 7);
    tileY = clamp(tileY, 0, 7);

    // Check if this is the selected tile
    bool isSelected = (tileX == u_SelectedTile.x && tileY == u_SelectedTile.y);

    // Sample the texture color
    vec4 textureColor = texture(u_floorTextureSampler, v_Tcoords);

    // Checkerboard pattern
    //bool isBlack = ((tileX + tileY) % 2) == 0;

    // Set color based on tile type
    if (isSelected) {
        // Selected tile: Green
        fragColor = vec4(0.0, 0.9, 0.0, 1.0);
    }
    //else if (isBlack) {
        // Black tile
    //    fragColor = vec4(0.1, 0.1, 0.1, 1.0);
    //}
    //else if (!isBlack) {
        // White color
    //    fragColor = vec4(0.9, 0.9, 0.9, 1.0);
    //}

    else {
        fragColor = textureColor;    
    }
}
)";

#endif