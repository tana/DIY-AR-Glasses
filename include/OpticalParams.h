#pragma once

#include "raylib-cpp.hpp"

const int DEFAULT_DISPLAY_WIDTH = 240;
const int DEFAULT_DISPLAY_HEIGHT = 240;

const float DEFAULT_INTERPUPILLARY_DISTANCE = 0.063f; // 63 mm

// Optical parameters of one eye
struct EyeParameters
{
  // Focal length in pixels
  float focalLength = 552.6f;

  // Optical center (principal point) in pixels
  // (0,0) is bottom left of the display (OpenGL's coordinate system, not OpenCV's one).
  raylib::Vector2 center = raylib::Vector2(DEFAULT_DISPLAY_WIDTH / 2.0f, DEFAULT_DISPLAY_HEIGHT / 2.0f);

  float k1 = 0.0f;  // Distortion coefficient k_1

  int displayWidth = DEFAULT_DISPLAY_WIDTH;
  int displayHeight = DEFAULT_DISPLAY_HEIGHT;
};

// Optical parameters of a whole head-mounted display
struct OpticalParams
{
  EyeParameters leftEye{
    552.6f,
    raylib::Vector2(DEFAULT_DISPLAY_WIDTH * (0.5f - 0.245f), DEFAULT_DISPLAY_HEIGHT * 0.5f),
    0.0f
  };

  // Column major order (unlike mathematical formula)
  Matrix leftEyeTransform{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    -DEFAULT_INTERPUPILLARY_DISTANCE / 2, 0.0f, 0.0f, 1.0f
  };

  EyeParameters rightEye{
    552.6f,
    raylib::Vector2(DEFAULT_DISPLAY_WIDTH * (0.5f + 0.245f), DEFAULT_DISPLAY_HEIGHT * 0.5f),
    0.0f
  };

  // Column major order (unlike mathematical formula)
  Matrix rightEyeTransform{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    DEFAULT_INTERPUPILLARY_DISTANCE / 2, 0.0f, 0.0f, 1.0f
  };
};