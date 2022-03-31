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

  raylib::Matrix attitudeMat = attitude.ToMatrix();
  rlMultMatrixf(reinterpret_cast<float*>(&attitudeMat));

  rlTranslatef(-pos.x, -pos.y, -pos.z);

  rlPushMatrix();
    rlTranslatef(0, 0, -1);
    rlRotatef(angle, 1.0f, 1.0f, 1.0f);

    DrawCubeWires(
      raylib::Vector3(0.0f, 0.0f, 0.0f),
      0.1f, 0.1f, 0.1f,
      WHITE);
  rlPopMatrix();

  // Draw axes of world coordinate frame
  rlPushMatrix();
    DrawLine3D(raylib::Vector3(0, 0, 0), raylib::Vector3(1, 0, 0), RED);
    DrawLine3D(raylib::Vector3(0, 0, 0), raylib::Vector3(0, 1, 0), GREEN);
    DrawLine3D(raylib::Vector3(0, 0, 0), raylib::Vector3(0, 0, 1), BLUE);
  rlPopMatrix();
}
