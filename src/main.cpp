#include <memory>
#include "raylib-cpp.hpp"
#include "constants.h"
#include "App.h"

int main()
{
  raylib::Window window(2 * DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
  SetTargetFPS(60);

  raylib::RenderTexture textureLeft(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  raylib::RenderTexture textureRight(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  auto app = std::make_shared<App>();

  while (!window.ShouldClose()) {
    app->update();

    // Render left eye
    textureLeft.BeginMode();
    app->draw(Eye::LEFT);
    textureLeft.EndMode();

    // Render right eye
    textureRight.BeginMode();
    app->draw(Eye::RIGHT);
    textureRight.EndMode();

    // Render to the window
    BeginDrawing();
    // TODO: distortion correction
    DrawTexture(textureLeft.texture, 0, 0, WHITE);
    DrawTexture(textureRight.texture, DISPLAY_WIDTH, 0, WHITE);
    EndDrawing();
  }

  return 0;
}
