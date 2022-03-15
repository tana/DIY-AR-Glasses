#pragma once

#include "App.h"

class App3D : public App
{
public:
  virtual ~App3D() override = default;

  void update() override;

  void draw(Eye eye, int displayWidth, int displayHeight) const override;

protected:
  virtual void draw3D(Eye eye) const;
};