#include "CalibrationApp.h"

#include "raylib-cpp.hpp"
#include "MenuApp.h"

void CalibrationApp::update()
{
  App::update();

  if (isOperationStarted(Operation::CANCEL)) {
    changeApp(std::make_shared<MenuApp>());
  }
}

void CalibrationApp::draw(Eye eye) const
{
  App::draw(eye);

  ClearBackground(BLACK);


}
