#ifndef __CHESSBOARD_FRAGMENT_H_
#define __CHESSBOARD_FRAGMENT_H_

#include <string>

const std::string chessboardFragmentShaderSrc = R"(
#version 430 core

in vec2 v_GridPos;              // Revieced from vertex shader (0, 1) in range

uniform ivec2 u_SelectedTile;   // Selected tile coordinates

out vec4 fragColor;

void main()
{
    // Convert continous position (0, 1) to tile coordinates (0, 7)
    int tileX = int(floor(v_GridPos.x * 8.0));
    int tileY = int(floor(v_GridPos.y * 8.0));

    // Clamp to valid range
    tileX = clamp(tileX, 0, 7);
    tileY = clamp(tileY, 0, 7);

    // Check if this is the selected tile
    bool isSelected = (tileX == u_SelectedTile.x && tileY == u_SelectedTile.y);

    // Checkerboard pattern
    bool isWhite = ((tileX + tileY) % 2) == 0;

    // Set color based on tile type
    if (isSelected) {
        // Selected tile: Green
        fragColor = vec4(0.0, 0.9, 0.0, 1.0);
    }
    elseif (isWhite) {
        // White tile
        fragColor = vec4(0.9, 0.9, 0.9, 1.0)
    }
    elseif (!isWhite) {
        // Black color
        fragColor = vec4(0.1, 0.1, 0.1, 1.0)
    }
}



)";

#endif