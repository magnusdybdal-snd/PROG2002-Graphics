#ifndef CHESS_ASSIGNMENT_FRAGMENT_H_
#define CHESS_ASSIGNMENT_FRAGMENT_H_

#include <string>

#ifdef __APPLE__   // macOS: 4.1 core
    constexpr const char* GLSL_CHESS_FRAGMENT_VERSION = "#version 410 core\n uniform sampler2D u_FloorTextureSampler;";
#else
    constexpr const char* GLSL_CHESS_FRAGMENT_VERSION = "#version 430 core\n layout(location = 0) uniform sampler2D u_FloorTextureSampler;";
#endif

inline std::string chessboardFragmentShaderSrc = std::string(GLSL_CHESS_FRAGMENT_VERSION) + R"(


in vec2 v_GridPos;                                              // INPUT:  Interpolated from vertex shader (0 to 1)
in vec2 v_TCoords;
in vec4 vs_FragPosition;        // INPUT: Position from vertex shader (world space)
in vec4 vs_Normal;              // INPUT: Normal position from vertex shader

uniform int u_GridSize;                                         // INPUT:  Board grid size set in c++ code
uniform ivec2 u_SelectedTile;                                   // INPUT:  From C++ code (which tile is selected)
uniform float u_ambientStrength = 1.0;
uniform vec3 u_lightSourcePosition;
uniform float u_diffuseStr;
uniform int u_TextureEnabled;
out vec4 fragColor;                                             // OUTPUT: Final pixel color

void main()
{
    vec4 chessboardColor;
    vec4 textureColor = texture(u_FloorTextureSampler, v_TCoords);

    vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_FragPosition.xyz));
    float diffuseStrength = max(dot(lightDirection, vs_Normal.xyz), 0.0) * u_diffuseStr;

    // Convert continous position (0, 1) to tile coordinates (0, 7)
    int tileX = int(floor(v_GridPos.x * float(u_GridSize)));
    int tileY = int(floor(v_GridPos.y * float(u_GridSize)));

    // Clamp to valid range for safety
    tileX = clamp(tileX, 0, u_GridSize - 1);
    tileY = clamp(tileY, 0, u_GridSize - 1);


    // Checkerboard pattern
    bool isBlack = ((tileX + tileY) % 2) == 0;

    // Check if this is the selected tile
    bool isSelected = (tileX == u_SelectedTile.x && tileY == u_SelectedTile.y);

    // Set color based on tile type

     if (isSelected) {
        // Selected tile: Green
        fragColor = vec4(0.2, 0.6, 0.2, 1.0) * (u_ambientStrength + diffuseStrength);
     } else {
        if (isBlack) {
            // Black tile
            chessboardColor = vec4(0.1, 0.1, 0.1, 1.0);
        }
        else {
            // White color
            chessboardColor = vec4(0.9, 0.9, 0.9, 1.0);
        }

        if (u_TextureEnabled == 0){
            fragColor = chessboardColor * (u_ambientStrength + diffuseStrength);
        } else {
            fragColor = mix(chessboardColor, textureColor, 0.7) * (u_ambientStrength + diffuseStrength);
        }
    }
}
)";

#endif // CHESS_ASSIGNMENT_FRAGMENT_H_