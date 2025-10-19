#pragma once

#include <vector>
#include <array>

namespace GeometricTools
{
    constexpr std::array<float, 3*2> UnitTriangle2D = {
        -0.5f, -0.5f,   // Bottom left
         0.5f, -0.5f,   // Bottom right
         0.0f,  0.5f }; // Top center
}