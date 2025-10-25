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

    // Unit cube (8 vertices x 3 components = 24 floats)
    constexpr std::array<float, 8*3> UnitCubeGeometry3D = {

        // Front facing square
        -0.5f, -0.5f, 0.5f,     // Bottom left front
         0.5f, -0.5f, 0.5f,     // Bottom right front
         0.5f,  0.5f, 0.5f,     // Top right front
        -0.5f,  0.5f, 0.5f,      // Top left front

        // Back facing square
        -0.5f, -0.5f, -0.5f,    // Bottom left back
         0.5f, -0.5f, -0.5f,    // Bottom right back
         0.5f,  0.5f, -0.5f,    // Top right back
        -0.5f,  0.5f, -0.5f     // Top left back
    };

    // Unit cube topology - 6 faces x 2 triangles x 3 indices = 36 indices
    constexpr std::array<unsigned int, 36> UnitCubeTopologyTriangles = {

        // Front face
        0, 1, 2,    2, 3, 0,
        // Right face
        1, 5, 6,    6, 2, 1,
        // Back face
        5, 4, 7,    7, 6, 5,
        // Left face
        4, 0, 3,    3, 7, 4,
        // Top face
        3, 2, 6,    6, 7, 3,
        // Bottom face
        4, 5, 1,    1, 0, 4
    };

    // Configurable template
    template<int DivisionsX, int DivisionsY>
    constexpr auto UnitGridGeometry2D() {

        // Number of verticies in each direction (x,y) will always be divisions + 1 (2x2 grid have 3 points)
        constexpr int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        // 2 floats for each coordinate
        constexpr int numFloats = numVertices * 2;

        // Array to hold verticies
        std::array<float, numFloats> vertices {};
        // Step distance for each split on the grid in each direction
        constexpr float stepX = 1.0f / DivisionsX;
        constexpr float stepY = 1.0f / DivisionsY;

        int index = 0;
        // Insert all verticies in the array
        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 
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

    template<unsigned int DivisionsX, unsigned int DivisionsY>
    constexpr auto UnitGridGeometry2DWTCoords() {

        // Number of verticies in each direction (x,y) will always be divisions + 1 (2x2 grid have 3 points)
        constexpr int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        // 4 floats for each coordinate (2 for pos and 2 for texture)
        constexpr int numFloats = numVertices * 4;

        // Array to hold verticies
        std::array<float, numFloats> vertices {};

        // Step distance for each split on the grid in each direction
        constexpr float stepX = 1.0f / DivisionsX;
        constexpr float stepY = 1.0f / DivisionsY;

        int index = 0;
        // Insert all verticies with interleaved position and texture coord in the array
        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                // Position coordinates (range -0.5 to 0.5)
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 

                // Texture coordinates (range 0.0 to 1.0)
                vertices[index++] = static_cast<float>(x) / DivisionsX;
                vertices[index++] = static_cast<float>(y) / DivisionsY;
            }
        }
        return vertices;
    }

    inline std::vector<float> UnitGridGeometry2DWTCoords(unsigned int DivisionsX, unsigned int DivisionsY) {

        int numVertices = (DivisionsX + 1) * (DivisionsY + 1);
        int numFloats = numVertices * 4;

        std::vector<float> vertices(numFloats);

        float stepX = 1.0f / DivisionsX;
        float stepY = 1.0f / DivisionsY;

        int index = 0;

        for (int y = 0; y <= DivisionsY; ++y) {
            for (int x = 0; x <= DivisionsX; ++x) {
                // Position coordinates (range -0.5 to 0.5)
                vertices[index++] = -0.5f + x * stepX; 
                vertices[index++] = -0.5f + y * stepY; 

                // Texture coordinates (range 0.0 to 1.0)
                vertices[index++] = static_cast<float>(x) / DivisionsX;
                vertices[index++] = static_cast<float>(y) / DivisionsY;
            }
        }
        return vertices;
    }
}
