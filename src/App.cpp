#include "App.h"

#include <iostream>
#include "raylib-cpp.hpp"
#include "constants.h"

// To find these values for a specific controller, use jstest tool
const int BUTTON_LEFT = 0;
const int BUTTON_RIGHT = 3;
const int BUTTON_UP = 2;
const int BUTTON_DOWN = 1;
const int BUTTON_OK = 15;
const int BUTTON_CANCEL = 14;

void App::update()
{
  processInput();
}

void App::draw(Eye eye) const
{
}

std::shared_ptr<App> App::getNextApp() const
{
  return nextApp;
}

bool App::isOperationStarted(Operation op) const
{
  return (prevOperations.find(op) == prevOperations.end()) && (operations.find(op) != operations.end());
}

bool App::isOperationActive(Operation op) const
{
  return operations.find(op) != operations.end();
}

void App::changeApp(std::shared_ptr<App> next)
{
  nextApp = next;
}

void App::processInput()
{
  prevOperations = operations;  // Copy

  bool gamepadFound = false;

  for (int gamepad = 0; IsGamepadAvailable(gamepad); ++gamepad) {
    gamepadFound = true;

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

    if (IsGamepadButtonDown(gamepad, BUTTON_OK)) {
      operations.insert(Operation::OK);
    } else {
      operations.erase(Operation::OK);
    }

    if (IsGamepadButtonDown(gamepad, BUTTON_CANCEL)) {
      operations.insert(Operation::CANCEL);
    } else {
      operations.erase(Operation::CANCEL);
    }
  }

  // Keyboard operation for development on PC
  // Use keyboard only when gamepad is not available
  if (!gamepadFound) {
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

    if (IsKeyDown(KEY_ENTER)) {
      operations.insert(Operation::OK);
    } else {
      operations.erase(Operation::OK);
    }

    if (IsKeyDown(KEY_BACKSPACE)) {
      operations.insert(Operation::CANCEL);
    } else {
      operations.erase(Operation::CANCEL);
    }
  }
}
