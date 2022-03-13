#pragma once

#include <vector>
#include <string>
#include <utility>
#include <functional>
#include "App.h"

class MenuApp : public App
{
public:
  MenuApp();
  
  virtual ~MenuApp() override = default;

  void update() override;

  void draw(Eye eye, int displayWidth, int displayHeight) const override;

private:
  void goToCalibration();
  void shutdown();

  const std::vector<std::pair<std::string, std::function<void()>>> items;

  const int FONT_SIZE = 20;

  int selectedItem = 0;
};