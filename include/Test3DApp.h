#pragma once

#include "App3D.h"

class Test3DApp : public App3D
{
public:
  virtual ~Test3DApp() override = default;

  void update() override;

protected:
  void draw3D(Eye eye) const override;

private:
  float angle = 0.0f;
};