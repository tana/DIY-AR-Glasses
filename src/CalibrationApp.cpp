#include "CalibrationApp.h"

#include <vector>
#include <string>
#include <stdexcept>
#include "raylib-cpp.hpp"
#include <fmt/core.h>
#include "constants.h"
#include "MenuApp.h"

const int FONT_SIZE = 20;

const std::vector<std::string> paramNames = {
  "Left k1", "Right k1",
  "Left center x", "Right center x", "Left center y", "Right center y"
};
const std::vector<float> paramSteps = {
  0.1f, 0.1f,
  1.0f, 1.0f, 1.0f, 1.0f
};

void CalibrationApp::update()
{
  App::update();

  if (isOperationStarted(Operation::UP)) {
    selected = std::max(selected - 1, 0);
  }
  if (isOperationStarted(Operation::DOWN)) {
    selected = std::min(selected + 1, static_cast<int>(paramNames.size() - 1));
  }

  if (auto params = opticalParams.lock()) { // Check whether the object behind weak_ref is available
    float val = getSelectedParameter(params.get(), selected);

    if (isOperationStarted(Operation::LEFT)) {
      setSelectedParameter(params.get(), selected, val - paramSteps[selected]);
    }
    if (isOperationStarted(Operation::RIGHT)) {
      setSelectedParameter(params.get(), selected, val + paramSteps[selected]);
    }
  }

  if (isOperationStarted(Operation::CANCEL)) {
    changeApp(std::make_shared<MenuApp>());
  }
}

void CalibrationApp::draw(Eye eye) const
{
  App::draw(eye);

  ClearBackground(BLACK);

  const int div = 8;
  for (int i = 1; i < div; ++i) {
    // Horizontal line
    float y = i * float(DISPLAY_HEIGHT) / div;
    DrawLineV(raylib::Vector2(0, y), raylib::Vector2(DISPLAY_WIDTH, y), WHITE);

    // Vertical line
    float x = i * float(DISPLAY_WIDTH) / div;
    DrawLineV(raylib::Vector2(x, 0), raylib::Vector2(x, DISPLAY_HEIGHT), WHITE);
  }

  if (auto params = opticalParams.lock()) { // Check whether the object behind weak_ref is available
    auto text = fmt::format("{} = {:2.2f}", paramNames[selected], getSelectedParameter(params.get(), selected));
    int textWidth = raylib::MeasureText(text, FONT_SIZE);
    raylib::DrawText(text, DISPLAY_WIDTH / 2 - textWidth / 2, DISPLAY_HEIGHT / 2 - FONT_SIZE / 2, FONT_SIZE, WHITE);
  }
}

float CalibrationApp::getSelectedParameter(const OpticalParams* params, int sel) const
{
  switch (sel) {
  case 0:
    return params->leftLens.k1;
  case 1:
    return params->rightLens.k1;
  case 2:
    return params->leftLens.center.x;
  case 3:
    return params->rightLens.center.x;
  case 4:
    return params->leftLens.center.y;
  case 5:
    return params->rightLens.center.y;
  default:
    throw std::out_of_range("sel is out of range");
  }
}

void CalibrationApp::setSelectedParameter(OpticalParams* params, int sel, float val)
{
  switch (sel) {
  case 0:
    params->leftLens.k1 = val;
    break;
  case 1:
    params->rightLens.k1 = val;
    break;
  case 2:
    params->leftLens.center.x = val;
    break;
  case 3:
    params->rightLens.center.x = val;
    break;
  case 4:
    params->leftLens.center.y = val;
    break;
  case 5:
    params->rightLens.center.y = val;
    break;
  default:
    throw std::out_of_range("sel is out of range");
  }
}
