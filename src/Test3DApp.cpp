#include "Test3DApp.h"

#include "raylib-cpp.hpp"
#include "rlgl.h"

void Test3DApp::update()
{
  App3D::update();

  angle = fmodf(angle + 1.0f, 360.0f);

  const float speed = 0.01f;

  if (isOperationActive(Operation::LEFT)) {
    pos.x -= speed;
  }
  if (isOperationActive(Operation::RIGHT)) {
    pos.x += speed;
  }
  if (isOperationActive(Operation::UP)) {
    pos.z -= speed;
  }
  if (isOperationActive(Operation::DOWN)) {
    pos.z += speed;
  }
}

void Test3DApp::draw3D(Eye eye) const
{
  App3D::draw3D(eye);

  ClearBackground(BLACK);

  rlTranslatef(pos.x, pos.y, pos.z);
  rlRotatef(angle, 1.0f, 1.0f, 1.0f);

  DrawCubeWires(
    raylib::Vector3(0.0f, 0.0f, 0.0f),
    0.1f, 0.1f, 0.1f,
    WHITE);
}
