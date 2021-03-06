#include "App3D.h"

#include "rlgl.h"
#include "raymath.h"
#include "MenuApp.h"

void App3D::update()
{
  App::update();

  if (isOperationStarted(Operation::CANCEL)) {
    changeApp(std::make_shared<MenuApp>());
  }
}

void App3D::draw(Eye eye, int displayWidth, int displayHeight) const
{
  App::draw(eye, displayWidth, displayHeight);

  if (const auto params = opticalParams.lock()) {
    EyeParameters eyeParams = (eye == Eye::LEFT) ? params->leftEye : params->rightEye;
    Matrix eyeTransform = (eye == Eye::LEFT) ? params->leftEyeTransform : params->rightEyeTransform;
    
    // Because asymmetric frustum is required,
    // custom settings are used instead of BeginMode3D.
    // (See: https://github.com/raysan5/raylib/blob/4954778f684b41046e72743fc32d29c74ea639f4/src/rcore.c#L2141 )

    double left = -eyeParams.center.x * RL_CULL_DISTANCE_NEAR / eyeParams.focalLength;
    double right = (displayWidth - double(eyeParams.center.x)) * RL_CULL_DISTANCE_NEAR / eyeParams.focalLength;
    double bottom = -eyeParams.center.y * RL_CULL_DISTANCE_NEAR / eyeParams.focalLength;
    double top = (displayHeight - double(eyeParams.center.y)) * RL_CULL_DISTANCE_NEAR / eyeParams.focalLength;

    rlMatrixMode(RL_PROJECTION);
    Matrix oldProjMat = rlGetMatrixProjection();  // Store old (2D) projection matrix
    rlLoadIdentity();
    rlFrustum(left, right, bottom, top, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);

    Matrix oldModelviewMat = rlGetMatrixModelview();  // Store old (2D) modelview matrix
    rlSetMatrixModelview(MatrixInvert(eyeTransform));
    rlMatrixMode(RL_MODELVIEW); // Without this, nothing will be rendered.

    draw3D(eye);

    rlDrawRenderBatchActive();

    // Restore matrices
    rlSetMatrixProjection(oldProjMat);
    rlSetMatrixModelview(oldModelviewMat);
  }
}

void App3D::draw3D(Eye eye) const
{
}
