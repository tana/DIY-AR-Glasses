#pragma once

#include "raylib-cpp.hpp"

// Madgwick filter for MARG sensor fusion
// Reference:
// S. O. H. Madgwick, A. J. L. Harrison and R. Vaidyanathan, "Estimation of IMU and MARG orientation using a gradient descent algorithm," 2011 IEEE International Conference on Rehabilitation Robotics, 2011, pp. 1-7, doi: 10.1109/ICORR.2011.5975346.
class MadgwickFilter
{
public:
  // Constructor
  MadgwickFilter(float deltaTime, float beta = 0.1f);

  void update(const raylib::Vector3& gyro, const raylib::Vector3& accel, const raylib::Vector3& mag);

  float deltaTime;
  float beta;

  // As in Madgwick's paper, this "attitude" means "attitude of Earth frame relative to sensor frame".
  // Theferore, v_E = q * v_S * conj(q) converts a sensor-frame vector v_S into an Earth-frame vector v_E,
  // and v_S = conj(q) * v_E * q converts an Earth-frame vector into a sensor-frame vector v_S.
  raylib::Quaternion attitude;
};
