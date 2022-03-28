#include <memory>
#include <iostream>

#include "raylib-cpp.hpp"
#include <fmt/core.h>

#include "App.h"
#include "MenuApp.h"
#include "OpticalParams.h"
#include "I2C.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "MadgwickFilter.h"

const int FRAME_RATE = 60;
const int I2C_BUS_NUM = 1;  // /dev/i2c-1

const raylib::Vector3 MAG_OFFSET(0.0054776245, -0.15919901, -0.041336596);

int main()
{
  I2C i2c(I2C_BUS_NUM);
  MPU6050 imu(&i2c);
  HMC5883L mag(&i2c);
  mag.setOutputRate(HMC5883L::DataOutputRate::RATE_75_HZ);

  MadgwickFilter filter(1.0f / FRAME_RATE, 0.1f);

  auto opticalParams = std::make_shared<OpticalParams>();
  
  if ((opticalParams->leftEye.displayWidth != opticalParams->rightEye.displayWidth)
    || (opticalParams->leftEye.displayHeight != opticalParams->rightEye.displayHeight)) {
    TraceLog(LOG_ERROR, "Display size of both displays must be the same");
    std::exit(-1);
  }

  const int displayWidth = opticalParams->leftEye.displayWidth;
  const int displayHeight = opticalParams->leftEye.displayHeight;

  raylib::Window window(2 * displayWidth, displayHeight, "");
  SetTargetFPS(FRAME_RATE);

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
    // Read sensor values
    imu.read();
    mag.read();
    // Convert to device coordinate frame (x is right, y is up, z is back)
    raylib::Vector3 gyroMeasure(-imu.angVel[1], imu.angVel[0], imu.angVel[2]);
    raylib::Vector3 accelMeasure(-imu.accel[1], imu.accel[0], imu.accel[2]);
    raylib::Vector3 magMeasure(mag.field[0] - MAG_OFFSET.x, mag.field[1] - MAG_OFFSET.y, mag.field[2] - MAG_OFFSET.z);
    //raylib::Vector3 gyroMeasure(0, 0, 0);
    //raylib::Vector3 accelMeasure(0, 1, 0);
    //raylib::Vector3 magMeasure(0, 0, -1);
    //fmt::print("({},{},{}), ({},{},{}), ({},{},{})\n", gyroMeasure.x, gyroMeasure.y, gyroMeasure.z, accelMeasure.x, accelMeasure.y, accelMeasure.z, magMeasure.x, magMeasure.y, magMeasure.z);
    // Estimate attitude from sensor measurements
    filter.deltaTime = GetFrameTime();  // Use actual current frame rate
    filter.update(gyroMeasure * DEG2RAD, accelMeasure, magMeasure);
    // Convert from z-up used in Madgwick filter into more graphics-friendly y-up coordinate system
    // Note: this matrix is written in column-major order
    auto attitudeYUp = raylib::Quaternion::FromMatrix(raylib::Matrix{
      1, 0, 0, 0,
      0, 0, -1, 0,
      0, -1, 0, 0,
      0, 0, 0, 1
    }) * filter.attitude;

    auto euler = attitudeYUp.ToEuler() * RAD2DEG;
    fmt::print("{: 3.0f},{: 3.0f},{: 3.0f}\n", euler.x, euler.y, euler.z);

    // Handle app change
    auto next = app->getNextApp();
    if (next) {
      app = next;
      app->opticalParams = opticalParams;
    }

    // Pass attitude to current app
    app->attitude = attitudeYUp;

    // Update state of current app
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
        const raylib::Vector2 centerLeft = opticalParams->leftEye.center / raylib::Vector2(displayWidth, displayHeight);
        shader.SetValue(centerLoc, &centerLeft, SHADER_UNIFORM_VEC2);
        shader.SetValue(k1Loc, &opticalParams->leftEye.k1, SHADER_UNIFORM_FLOAT);
        // Render textures have to be vertically flipped when drawing.
        DrawTextureRec(
          textureLeft.texture,
          raylib::Rectangle(0, 0, displayWidth, -displayHeight),
          raylib::Vector2(0, 0),
          WHITE);
      shader.EndMode();

      // Right
      shader.BeginMode();
        const raylib::Vector2 centerRight = opticalParams->rightEye.center / raylib::Vector2(displayWidth, displayHeight);
        shader.SetValue(centerLoc, &centerRight, SHADER_UNIFORM_VEC2);
        shader.SetValue(k1Loc, &opticalParams->rightEye.k1, SHADER_UNIFORM_FLOAT);
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
