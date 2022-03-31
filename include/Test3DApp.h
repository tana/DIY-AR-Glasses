#pragma once

#include "App3D.h"

#include "raylib-cpp.hpp"

class Test3DApp : public App3D
{
public:
  virtual ~Test3DApp() override = default;

  void update() override;

protected:
  void draw3D(Eye eye) const override;

private:
  float angle = 0.0f;
  raylib::Vector3 pos{ 0.0f, 0.0f, 0.0f };  // Position of the device in world coordinate frame
};
