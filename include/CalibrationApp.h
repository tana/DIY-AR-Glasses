#pragma once

#include "App.h"
#include "OpticalParams.h"

class CalibrationApp : public App
{
public:
  virtual ~CalibrationApp() override = default;

  void update() override;

  void draw(Eye eye) const override;

private:
  float getSelectedParameter(const OpticalParams* params, int sel) const;
  void setSelectedParameter(OpticalParams* params, int sel, float val);

  int selected = 0;
};