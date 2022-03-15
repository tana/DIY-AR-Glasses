#include "Test3DApp.h"

#include "raylib-cpp.hpp"
#include "rlgl.h"

void Test3DApp::update()
{
  App3D::update();

  angle = fmodf(angle + 1.0f, 360.0f);
}

void Test3DApp::draw3D(Eye eye) const
{
  App3D::draw3D(eye);

  ClearBackground(BLACK);

  rlTranslatef(0.0f, 0.0f, -1.0f);
  rlRotatef(angle, 1.0f, 1.0f, 1.0f);

  DrawCubeWires(
    raylib::Vector3(0.0f, 0.0f, 0.0f),
    0.1f, 0.1f, 0.1f,
    WHITE);
}
