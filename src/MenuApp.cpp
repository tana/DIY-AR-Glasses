#include "MenuApp.h"

#include <utility>
#include <iostream>
#include "raylib-cpp.hpp"
#include "Test3DApp.h"
#include "CalibrationApp.h"

MenuApp::MenuApp()
  : App()
  , items{
      { "3D test", std::bind(&MenuApp::goTo3DTest, this) },
      { "Calibration", std::bind(&MenuApp::goToCalibration, this) },
      { "Shutdown", std::bind(&MenuApp::shutdown, this) }
    }
{
}

void MenuApp::update()
{
  App::update();

  if (isOperationStarted(Operation::UP)) {
    selectedItem = std::max(0, selectedItem - 1);
  }
  if (isOperationStarted(Operation::DOWN)) {
    selectedItem = std::min(selectedItem + 1, static_cast<int>(items.size() - 1));
  }

  if (isOperationStarted(Operation::OK)) {
    items[selectedItem].second();
  }
}

void MenuApp::draw(Eye eye, int displayWidth, int displayHeight) const
{
  App::draw(eye, displayWidth, displayHeight);

  ClearBackground(BLACK);

  int i = 0;
  for (const auto& [name, func] : items) {
    int x = displayWidth / 2 - raylib::MeasureText(name, FONT_SIZE) / 2;
    int y = FONT_SIZE + FONT_SIZE * i;
    raylib::DrawText(name, x, y, FONT_SIZE, WHITE);

    if (i == selectedItem) {
      DrawText(">", 20, y, FONT_SIZE, WHITE);
    }

    ++i;
  }
}

void MenuApp::goTo3DTest()
{
  changeApp(std::make_shared<Test3DApp>());
}

void MenuApp::goToCalibration()
{
  changeApp(std::make_shared<CalibrationApp>());
}

void MenuApp::shutdown()
{
  //std::system("shutdown -h +1");  // Shutdown the system after one minute
  exit(0);
}
