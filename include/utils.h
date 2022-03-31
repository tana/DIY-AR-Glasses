#pragma once

#include "raylib-cpp.hpp"

// Conversion from a 3-component vector to a quaternion
inline raylib::Quaternion vec3ToQuat(const Vector3& v)
{
  return raylib::Quaternion(v.x, v.y, v.z, 0.0f);
}

// Instead of Raylib's QuaternionScale which seems wrong
inline raylib::Quaternion myQuaternionScale(const Quaternion& q, const float& s)
{
  return raylib::Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
}

// Quaternion operators not defined in raylib-cpp

inline raylib::Quaternion operator*(const float& s, const Quaternion& q)
{
  return myQuaternionScale(q, s);
}

inline raylib::Quaternion operator*(const Quaternion& q, const float& s)
{
  return myQuaternionScale(q, s);
}

inline raylib::Quaternion operator+(const Quaternion& q1, const Quaternion& q2)
{
  return QuaternionAdd(q1, q2);
}

inline raylib::Quaternion operator-(const Quaternion& q1, const Quaternion& q2)
{
  return QuaternionSubtract(q1, q2);
}
