#include <memory>
#include "raylib-cpp.hpp"
#include "App.h"
#include "MenuApp.h"
#include "OpticalParams.h"

int main()
{
  auto opticalParams = std::make_shared<OpticalParams>();
  
  if ((opticalParams->leftLens.displayWidth != opticalParams->rightLens.displayWidth)
    || (opticalParams->leftLens.displayHeight != opticalParams->rightLens.displayHeight)) {
    TraceLog(LOG_ERROR, "Display size of both displays must be the same");
    std::exit(-1);
  }

  const int displayWidth = opticalParams->leftLens.displayWidth;
  const int displayHeight = opticalParams->leftLens.displayHeight;

  raylib::Window window(2 * displayWidth, displayHeight, "");
  SetTargetFPS(60);

  raylib::RenderTexture textureLeft(displayWidth, displayHeight);
  raylib::RenderTexture textureRight(displayWidth, displayHeight);

  SetTextureFilter(textureLeft.texture, TEXTURE_FILTER_BILINEAR);
  SetTextureFilter(textureRight.texture, TEXTURE_FILTER_BILINEAR);

  // Load lens distortion compensation shader
  // nullptr means default vertex shader
  // (See: https://github.com/raysan5/raylib/blob/8065504aba66eeddc69111508ef8a267358467a6/examples/shaders/shaders_postprocessing.c#L86 )
  raylib::Shader shader(nullptr, "shaders/distortion.frag");
  int centerLoc = shader.GetLocation("center");
  int k1Loc = shader.GetLocation("k1");

  std::shared_ptr<App> app = std::make_shared<MenuApp>();
  app->opticalParams = opticalParams;

  while (!window.ShouldClose()) {
    auto next = app->getNextApp();
    if (next) {
      app = next;
      app->opticalParams = opticalParams;
    }

    app->update();

    // Render left eye
    textureLeft.BeginMode();
      app->draw(Eye::LEFT, displayWidth, displayHeight);
    textureLeft.EndMode();

    // Render right eye
    textureRight.BeginMode();
      app->draw(Eye::RIGHT, displayWidth, displayHeight);
    textureRight.EndMode();

    // Render to the window
    BeginDrawing();
      // Left
      shader.BeginMode();
        const raylib::Vector2 centerLeft = opticalParams->leftLens.center / raylib::Vector2(displayWidth, displayHeight);
        shader.SetValue(centerLoc, &centerLeft, SHADER_UNIFORM_VEC2);
        shader.SetValue(k1Loc, &opticalParams->leftLens.k1, SHADER_UNIFORM_FLOAT);
        // Render textures have to be vertically flipped when drawing.
        DrawTextureRec(
          textureLeft.texture,
          raylib::Rectangle(0, 0, displayWidth, -displayHeight),
          raylib::Vector2(0, 0),
          WHITE);
      shader.EndMode();

      // Right
      shader.BeginMode();
        const raylib::Vector2 centerRight = opticalParams->rightLens.center / raylib::Vector2(displayWidth, displayHeight);
        shader.SetValue(centerLoc, &centerRight, SHADER_UNIFORM_VEC2);
        shader.SetValue(k1Loc, &opticalParams->rightLens.k1, SHADER_UNIFORM_FLOAT);
        DrawTextureRec(
          textureRight.texture,
          raylib::Rectangle(0, 0, displayWidth, -displayHeight),
          raylib::Vector2(displayWidth, 0),
          WHITE);
      shader.EndMode();
    EndDrawing();
  }

  return 0;
}
