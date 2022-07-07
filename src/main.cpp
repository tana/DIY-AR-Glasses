#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#include "raylib-cpp.hpp"

#include "App.h"
#include "MenuApp.h"
#include "OpticalParams.h"
#include "I2C.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "MadgwickFilter.h"

const int FRAME_RATE = 60;  // Hz
const int SENSOR_PROCESS_RATE = 1000;  // Hz
const int PROCESS_RATE_PRINT_INTERVAL = 10 * SENSOR_PROCESS_RATE;
const int I2C_BUS_NUM = 1;  // /dev/i2c-1

const raylib::Vector3 MAG_OFFSET(0.0054776245, -0.15919901, -0.041336596);
const raylib::Matrix IMU_TILT_CORRECT = raylib::Matrix::RotateX(20.0f * DEG2RAD);

int main()
{
  bool sensorThreadShouldStop = false;
  raylib::Quaternion sharedAttitude;  // For passing attitude from sensor thread to main thread
  std::mutex sharedAttitudeMutex; // For preventing modification of sharedAttitude while reading it

  // Process sensor data in a separate thread
  std::thread sensorThread([&] {
    I2C i2c(I2C_BUS_NUM);
    MPU6050 imu(&i2c);
    HMC5883L mag(&i2c);
    mag.setOutputRate(HMC5883L::DataOutputRate::RATE_75_HZ);

    MadgwickFilter filter(1.0f / FRAME_RATE, 0.1f);

    const auto targetDeltaTime = std::chrono::milliseconds(1000 / SENSOR_PROCESS_RATE);
    std::chrono::duration<double> deltaTime = targetDeltaTime;
    auto startTime = std::chrono::high_resolution_clock::now();
    auto lastPrintTime = startTime;
    int processCount = 0;

    while (!sensorThreadShouldStop) {
      // Read sensor values
      imu.read();
      mag.read();
      // Convert to device coordinate frame (x is right, y is up, z is back)
      raylib::Vector3 gyroMeasure(-imu.angVel[1] * DEG2RAD, imu.angVel[0] * DEG2RAD, imu.angVel[2] * DEG2RAD);
      raylib::Vector3 accelMeasure(-imu.accel[1], imu.accel[0], imu.accel[2]);
      raylib::Vector3 magMeasure(mag.field[0] - MAG_OFFSET.x, mag.field[1] - MAG_OFFSET.y, mag.field[2] - MAG_OFFSET.z);

      gyroMeasure = gyroMeasure.Transform(IMU_TILT_CORRECT);
      accelMeasure = accelMeasure.Transform(IMU_TILT_CORRECT);

      // Estimate attitude from sensor measurements
      filter.deltaTime = float(deltaTime.count()); // Use actual delta time for filtering
      filter.update(gyroMeasure, accelMeasure, magMeasure);

      // Send attitude to main thread
      {
        std::lock_guard<std::mutex> lock(sharedAttitudeMutex);
        sharedAttitude = filter.attitude;
      }

      ++processCount;

      std::this_thread::sleep_until(startTime + targetDeltaTime);

      const auto prevStartTime = startTime;
      startTime = std::chrono::high_resolution_clock::now();
      deltaTime = startTime - prevStartTime;

      // Print actual processing rate
      if (processCount >= PROCESS_RATE_PRINT_INTERVAL) {
        const std::chrono::duration<double> timeSinceLastPrint = startTime - lastPrintTime;
        const double processRate = double(processCount) / timeSinceLastPrint.count();

        TraceLog(LOG_INFO, "Actual sensor processing rate was %.1f Hz", processRate);

        lastPrintTime = startTime;
        processCount = 0;
      }
    }
  });

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

  auto zUpToYUp = raylib::Quaternion::FromAxisAngle(raylib::Vector3(1, 0, 0), -90.0f * DEG2RAD);

  while (!window.ShouldClose()) {
    // Handle app change
    auto next = app->getNextApp();
    if (next) {
      app = next;
      app->opticalParams = opticalParams;
    }

    // Receive attitude from sensor thread
    raylib::Quaternion attitude;
    {
      std::lock_guard<std::mutex> lock(sharedAttitudeMutex);
      attitude = sharedAttitude;
    }
    // Pass attitude to current app
    app->attitude = zUpToYUp * attitude;

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

  sensorThreadShouldStop = true;
  sensorThread.join();

  return 0;
}
