#include <iostream>
#include "raylib-cpp.hpp"

const int DISPLAY_WIDTH = 240;
const int DISPLAY_HEIGHT = 240;

int main()
{
  raylib::Window window(2 * DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
  SetTargetFPS(60);

  while (!window.ShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    EndDrawing();
  }

  return 0;
}
