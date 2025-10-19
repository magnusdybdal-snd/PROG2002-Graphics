#pragma once

#include <vector>
#include <array>

namespace GeometricTools
{
    // Unit triangle (3 vertices × 2 components = 6 floats)
    constexpr std::array<float, 3*2> UnitTriangle2D = { 
        -0.5f, -0.5f,  // Bottom left
         0.5f, -0.5f,  // Bottom right
         0.0f,  0.5f   // Top center
    };

    // Unit square (4 vertices × 2 components = 8 floats)
    constexpr std::array<float, 4*2> UnitSquare2D = {
        -0.5f, -0.5f,  // Bottom left
         0.5f, -0.5f,  // Bottom right
         0.5f,  0.5f,  // Top right
        -0.5f,  0.5f   // Top left
    };

    // Configurable template
    template<int DivisionsX, int DivisionsY>
    constexpr auto UnitGridGeometry2D() {

        // Number of verticies in each direction (x,y) will always be divisions + 1 (2x2 grid have 3 points)
        constexpr int numVerticies = (DivisionsX + 1) * (DivisionsY + 1);
        // 2 floats for each coordinate
        constexpr int numFloats = numVerticies * 2;

        // Array to hold verticies
        std::array<float, numFloats> verticies {};
        // Step distance for each split on the grid in each direction
        constexpr float stepX = 1.0f / DivisionsX;
        constexpr float stepY = 1.0f / DivisionsY;

        int index = 0;
        // Insert all verticies in the array
        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                verticies[index++] = -0.5f + x * stepX; 
                verticies[index++] = -0.5f + y * stepY; 
            }
        }

        return verticies;
    }

    template<int DivisionsX, int DivisionsY>
    constexpr auto UnitGridTopologyTriangles() {

        // Each cell becomes 2 tringles, each triangle has 3 indices
        constexpr int numCells = DivisionsX * DivisionsY;
        constexpr int numIndices = numCells * 2 * 3;

        // Array to hold the indices
        std::array<unsigned int, numIndices> indices{};

        int index = 0;

        // Loop trough each cell in the grid
        for (int y = 0; y < DivisionsY; ++y) {
            for (int x = 0; x < DivisionsX; ++x) {

                // Calculate the 4 corner vertex indices of the current cell
                unsigned int bottomLeft  = y * (DivisionsX + 1) + x;
                unsigned int bottomRight = y * (DivisionsX + 1) + (x + 1);
                unsigned int topLeft     = (y + 1) * (DivisionsY + 1) + x;
                unsigned int topRight    = (y + 1) * (DivisionsY + 1) + (x + 1); 

                // First triangle (bottom-left triangle of the cell)
                indices[index++] = bottomLeft;
                indices[index++] = bottomRight;
                indices[index++] = topLeft;

                // Second triangle (top-right triangle of the cell)
                indices[index++] = bottomRight;
                indices[index++] = topRight;
                indices[index++] = topLeft;
            }
        }

        return indices;
    }
}