#include <iostream>
#include "raylib-cpp.hpp"

const int DISPLAY_WIDTH = 240;
const int DISPLAY_HEIGHT = 240;

// To find these values for a specific controller, use jstest tool
const int BUTTON_LEFT = 0;
const int BUTTON_RIGHT = 3;
const int BUTTON_UP = 2;
const int BUTTON_DOWN = 1;

void update()
{
  for (int gamepad = 0; IsGamepadAvailable(gamepad); ++gamepad) {
    if (IsGamepadButtonDown(gamepad, BUTTON_LEFT)) {
      std::cout << "LEFT" << std::endl;
    }
    if (IsGamepadButtonDown(gamepad, BUTTON_RIGHT)) {
      std::cout << "RIGHT" << std::endl;
    }
    if (IsGamepadButtonDown(gamepad, BUTTON_UP)) {
      std::cout << "UP" << std::endl;
    }
    if (IsGamepadButtonDown(gamepad, BUTTON_DOWN)) {
      std::cout << "DOWN" << std::endl;
    }
  }
}

void draw()
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

int main()
{
  raylib::Window window(2 * DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
  SetTargetFPS(60);

  raylib::RenderTexture textureLeft(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  raylib::RenderTexture textureRight(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  while (!window.ShouldClose()) {
    update();

    // Render left eye
    textureLeft.BeginMode();
    draw();
    textureLeft.EndMode();

    // Render right eye
    textureRight.BeginMode();
    draw();
    textureRight.EndMode();

    // Render to the window
    BeginDrawing();
    DrawTexture(textureLeft.texture, 0, 0, WHITE);
    DrawTexture(textureRight.texture, DISPLAY_WIDTH, 0, WHITE);
    EndDrawing();
  }

  return 0;
}
