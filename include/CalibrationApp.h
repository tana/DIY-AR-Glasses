#pragma once

#include "App.h"

class CalibrationApp : public App
{
public:
  virtual ~CalibrationApp() override = default;

  void update() override;

  void draw(Eye eye) const override;
};