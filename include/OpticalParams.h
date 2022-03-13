#pragma once

#include "raylib-cpp.hpp"
#include "constants.h"

// Optical parameters of one lens
struct LensParameters
{
  raylib::Vector2 center = raylib::Vector2(DISPLAY_WIDTH / 2.0f, DISPLAY_HEIGHT / 2.0f);

  float k1 = 0.0f;  // Distortion coefficient k_1
};

// Optical parameters of a whole head-mounted display
struct OpticalParams
{
  LensParameters leftLens{ raylib::Vector2(DISPLAY_WIDTH * (0.5f - 0.245f), DISPLAY_HEIGHT * 0.5f), 0.0f };
  LensParameters rightLens{ raylib::Vector2(DISPLAY_WIDTH * (0.5f + 0.245f), DISPLAY_HEIGHT * 0.5f), 0.0f };
};