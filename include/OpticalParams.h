#pragma once

#include "raylib-cpp.hpp"

const int DEFAULT_DISPLAY_WIDTH = 240;
const int DEFAULT_DISPLAY_HEIGHT = 240;

// Optical parameters of one lens
struct LensParameters
{
  raylib::Vector2 center = raylib::Vector2(DEFAULT_DISPLAY_WIDTH / 2.0f, DEFAULT_DISPLAY_HEIGHT / 2.0f);

  float k1 = 0.0f;  // Distortion coefficient k_1

  int displayWidth = DEFAULT_DISPLAY_WIDTH;
  int displayHeight = DEFAULT_DISPLAY_HEIGHT;
};

// Optical parameters of a whole head-mounted display
struct OpticalParams
{
  LensParameters leftLens{ raylib::Vector2(DEFAULT_DISPLAY_WIDTH * (0.5f - 0.245f), DEFAULT_DISPLAY_HEIGHT * 0.5f), 0.0f };
  LensParameters rightLens{ raylib::Vector2(DEFAULT_DISPLAY_WIDTH * (0.5f + 0.245f), DEFAULT_DISPLAY_HEIGHT * 0.5f), 0.0f };
};