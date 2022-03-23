#include "MadgwickFilter.h"

#include <cmath>
#include "utils.h"

MadgwickFilter::MadgwickFilter(float deltaTime, float beta)
  : deltaTime(deltaTime), beta(beta)
{
}

void MadgwickFilter::update(const raylib::Vector3& gyro, const raylib::Vector3& accel, const raylib::Vector3& mag)
{
  // Main calculation of Madgwick filter.
  // Reference:
  // S. O. H. Madgwick, A. J. L. Harrison and R. Vaidyanathan, "Estimation of IMU and MARG orientation using a gradient descent algorithm," 2011 IEEE International Conference on Rehabilitation Robotics, 2011, pp. 1-7, doi: 10.1109/ICORR.2011.5975346.

  // Note: In raylib, scalar (real) part of a quaternion is w and vector (imaginary) part are x, y, z.
  // Note: Vector3RotateByQuaternion(v, q) means q*v*conj(q).

  // ------- Preprocessing of sensor measurement -------

  const auto accelNorm = accel.Normalize();
  const auto magNorm = mag.Normalize();

  // Compensate magnetic distortion using previous estimate of attitude
  const auto magEarth = Vector3RotateByQuaternion(magNorm, attitude);
  const raylib::Vector3 magComp(
    sqrtf(magEarth.x * magEarth.x + magEarth.y * magEarth.y),
    0,
    magEarth.z);

  // ------- Integration of gyroscope measurement -------

  // Time derivative of attitude calculated from angular rate (gyro measurement) only
  const auto attitudeChangeGyro = 0.5f * attitude * vec3ToQuat(gyro);

  // ------- Gradient descent calculation ------

  // Components of the objective function to minimize
  const float residual[6] = {
    // Difference from accelerometer measurement
    2*(attitude.x*attitude.z - attitude.w*attitude.y)        - accelNorm.x,
    2*(attitude.w*attitude.x + attitude.y*attitude.z)        - accelNorm.y,
    2*(0.5f - attitude.x*attitude.x - attitude.y*attitude.y) - accelNorm.z,
    // Difference from magnetometer measurement
    2*magComp.x*(0.5f - attitude.y*attitude.y - attitude.z*attitude.z) + 2*magComp.z*(attitude.x*attitude.z - attitude.w*attitude.y)        - magNorm.x,
    2*magComp.x*(attitude.x*attitude.y - attitude.w*attitude.z)        + 2*magComp.z*(attitude.w*attitude.x + attitude.y*attitude.z)        - magNorm.y,
    2*magComp.x*(attitude.w*attitude.y + attitude.x*attitude.z)        + 2*magComp.z*(0.5f - attitude.x*attitude.x - attitude.y*attitude.y) - magNorm.z
  };
  // jacobian[row][column] 
  const float jacobian[6][4] = {
    // Related to accelerometer measurement
    { -2*attitude.y,  2*attitude.z, -2*attitude.w,  2*attitude.x },
    {  2*attitude.x,  2*attitude.w,  2*attitude.z,  2*attitude.y },
    {             0, -4*attitude.x, -4*attitude.y,             0 },
    // Related to magnetometer measurement
    {                          -2*magComp.z*attitude.y,                          2*magComp.z*attitude.z, -4*magComp.x*attitude.y - 2*magComp.z*attitude.w, -4*magComp.x*attitude.z + 2*magComp.z*attitude.x },
    { -2*magComp.x*attitude.z + 2*magComp.z*attitude.x, 2*magComp.x*attitude.y + 2*magComp.z*attitude.w,  2*magComp.x*attitude.x + 2*magComp.z*attitude.z, -2*magComp.x*attitude.w + 2*magComp.z*attitude.y },
    {  2*magComp.x*attitude.y                         , 2*magComp.x*attitude.z - 4*magComp.z*attitude.x,  2*magComp.x*attitude.w - 4*magComp.z*attitude.y,  2*magComp.x*attitude.x                          }
  };

  // Multiply Jacobian with residuals to calculate gradient
  raylib::Quaternion gradient(0.0f, 0.0f, 0.0f, 0.0f);
  for (int i = 0; i < 6; ++i) {
    gradient.w += jacobian[i][0] * residual[i];
    gradient.x += jacobian[i][1] * residual[i];
    gradient.y += jacobian[i][2] * residual[i];
    gradient.z += jacobian[i][3] * residual[i];
  }

  // ------- Correct attitude -------
  const auto attitudeChange = attitudeChangeGyro - beta * gradient.Normalize();
  attitude = (attitude + attitudeChange * deltaTime).Normalize();
}
