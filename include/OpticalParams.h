#pragma once

// Optical parameters of one lens
struct LensParameters
{
  float k1 = 0.0f;  // Distortion coefficient k_1
};

// Optical parameters of a whole head-mounted display
struct OpticalParams
{
  LensParameters leftLens, rightLens;
};