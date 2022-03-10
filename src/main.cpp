#include <memory>
#include "raylib-cpp.hpp"
#include "constants.h"
#include "App.h"
#include "MenuApp.h"

int main()
{
  raylib::Window window(2 * DISPLAY_WIDTH, DISPLAY_HEIGHT, "");
  SetTargetFPS(60);

  raylib::RenderTexture textureLeft(DISPLAY_WIDTH, DISPLAY_HEIGHT);
  raylib::RenderTexture textureRight(DISPLAY_WIDTH, DISPLAY_HEIGHT);

  std::shared_ptr<App> app = std::make_shared<MenuApp>();

  while (!window.ShouldClose()) {
    auto next = app->getNextApp();
    if (next) {
      app = next;
    }

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
    // Render textures have to be vertically flipped when drawing.
    DrawTextureRec(
      textureLeft.texture,
      raylib::Rectangle(0, 0, DISPLAY_WIDTH, -DISPLAY_HEIGHT),
      raylib::Vector2(0, 0),
      WHITE);
    DrawTextureRec(
      textureLeft.texture,
      raylib::Rectangle(0, 0, DISPLAY_WIDTH, -DISPLAY_HEIGHT),
      raylib::Vector2(DISPLAY_WIDTH, 0),
      WHITE);
    EndDrawing();
  }

  return 0;
}
