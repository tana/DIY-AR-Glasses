#include "App.h"

#include <iostream>
#include "raylib-cpp.hpp"
#include "constants.h"

// To find these values for a specific controller, use jstest tool
const int BUTTON_LEFT = 0;
const int BUTTON_RIGHT = 3;
const int BUTTON_UP = 2;
const int BUTTON_DOWN = 1;

void App::update()
{
  processInput();

  if (isOperationStarted(Operation::LEFT)) {
    std::cout << "left" << std::endl;
  }
}

void App::draw(Eye eye) const
{
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
}

bool App::isOperationStarted(Operation op) const
{
  return (prevOperations.find(op) == prevOperations.end()) && (operations.find(op) != operations.end());
}

bool App::isOperationActive(Operation op) const
{
  return operations.find(op) != operations.end();
}

void App::processInput()
{
  prevOperations = operations;  // Copy

  for (int gamepad = 0; IsGamepadAvailable(gamepad); ++gamepad) {
    if (IsGamepadButtonDown(gamepad, BUTTON_LEFT)) {
      operations.insert(Operation::LEFT);
    } else {
      operations.erase(Operation::LEFT);
    }

    if (IsGamepadButtonDown(gamepad, BUTTON_RIGHT)) {
      operations.insert(Operation::RIGHT);
    } else {
      operations.erase(Operation::RIGHT);
    }

    if (IsGamepadButtonDown(gamepad, BUTTON_UP)) {
      operations.insert(Operation::UP);
    } else {
      operations.erase(Operation::UP);
    }

    if (IsGamepadButtonDown(gamepad, BUTTON_DOWN)) {
      operations.insert(Operation::DOWN);
    } else {
      operations.erase(Operation::DOWN);
    }
  }

  // Keyboard operation for development on PC

  if (IsKeyDown(KEY_LEFT)) {
    operations.insert(Operation::LEFT);
  } else {
    operations.erase(Operation::LEFT);
  }

  if (IsKeyDown(KEY_RIGHT)) {
    operations.insert(Operation::RIGHT);
  } else {
    operations.erase(Operation::RIGHT);
  }

  if (IsKeyDown(KEY_UP)) {
    operations.insert(Operation::UP);
  } else {
    operations.erase(Operation::UP);
  }

  if (IsKeyDown(KEY_DOWN)) {
    operations.insert(Operation::DOWN);
  } else {
    operations.erase(Operation::DOWN);
  }
}
