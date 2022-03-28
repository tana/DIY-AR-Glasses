#pragma once

#include "raylib-cpp.hpp"

// Conversion from a 3-component vector to a quaternion
inline raylib::Quaternion vec3ToQuat(const Vector3& v)
{
  return raylib::Quaternion{ .x = v.x, .y = v.y, .z = v.z, .w = 0.0f };
}

// Quaternion operators not defined in raylib-cpp

inline raylib::Quaternion operator*(const float& s, const Quaternion& q)
{
  return QuaternionScale(q, s);
}

inline raylib::Quaternion operator*(const Quaternion& q, const float& s)
{
  return QuaternionScale(q, s);
}

inline raylib::Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
  return QuaternionAdd(q1, q2);
}

inline raylib::Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
  return QuaternionSubtract(q1, q2);
}