#pragma once

#include "I2C.h"

// Driver for Invensense MPU-6050 accelerometer/gyroscope
// Reference:
//  "MPU-6000 and MPU-6050 Product Specification Revision 3.4", https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/mpu-6000-datasheet1.pdf
//  "MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2", https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
//  (available from https://product.tdk.com/ja/search/sensor/mortion-inertial/imu/info?part_no=MPU-6050 )
class MPU6050
{
public:
  enum class GyroRange : uint8_t
  {
    FULL_SCALE_250DPS = 0,
    FULL_SCALE_500DPS = 1,
    FULL_SCALE_1000DPS = 2,
    FULL_SCALE_2000DPS = 3
  };

  enum class AccelRange : uint8_t
  {
    FULL_SCALE_2G = 0,
    FULL_SCALE_4G = 1,
    FULL_SCALE_8G = 2,
    FULL_SCALE_16G = 3
  };

  // Constructor
  MPU6050(I2C* i2cBus, uint16_t addr = 0x68);

  // Read sensor values (acceleration, angular velocity, temperature)
  void read();

  // Set accelerometer range
  void setAccelRange(AccelRange range);
  // Set gyroscope range
  void setGyroRange(GyroRange range);

  float accel[3]; // Acceleration vector (in G)
  float angVel[3];  // Angular velocity (in deg/s)
  float temp;  // Temperature (in degrees Celcius)

  const float ANG_VEL_CONVERSION[4] = {
    250.0f / 32768,
    500.0f / 32768,
    1000.0f / 32768,
    2000.0f / 32768
  };

  const float ACCEL_CONVERSION[4] = {
    2.0f / 32768,
    4.0f / 32768,
    8.0f / 32768,
    16.0f / 32768,
  };

private:
  I2C* i2c;
  uint16_t devAddr;

  AccelRange accelRange;
  GyroRange gyroRange;
  
  // Register addresses
  static const uint8_t PWR_MGMT_1 = 0x6B;
  static const uint8_t WHO_AM_I = 0x75;
  static const uint8_t ACCEL_XOUT_H = 0x3B;
  static const uint8_t ACCEL_CONFIG = 0x1C;
  static const uint8_t GYRO_CONFIG = 0x1B;
};
