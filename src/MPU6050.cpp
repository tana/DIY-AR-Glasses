#include "MPU6050.h"

#include <stdexcept>
#include <thread>
#include <chrono>

MPU6050::MPU6050(I2C* i2cBus, uint16_t addr)
  : i2c(i2cBus), devAddr(addr)
{
  // Reset MPU6050
  i2c->writeByteReg(devAddr, PWR_MGMT_1, 0b10000000);
  // Wait a while after reset
  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  // Disable SLEEP mode
  i2c->writeByteReg(devAddr, PWR_MGMT_1, 0b00000000);

  // Test whether the host can properly communicate with MPU-6050
  if (i2c->readByteReg(devAddr, WHO_AM_I) != devAddr) {
    throw std::runtime_error("Incorrect WHO_AM_I value");
  }

  // Set default range
  setAccelRange(AccelRange::FULL_SCALE_2G);
  setGyroRange(GyroRange::FULL_SCALE_250DPS);
}

void MPU6050::read()
{
  const size_t readLen = 3 * 2 + 2 + 3 * 2;
  uint8_t buf[readLen];
  // Read acceleration, temperature, and angular velocity at once
  i2c->readReg(devAddr, ACCEL_XOUT_H, buf, readLen);

  // Convert values from big endian
  int16_t accelRaw[3] = {
    static_cast<int16_t>((buf[0] << 8) | buf[1]), // x
    static_cast<int16_t>((buf[2] << 8) | buf[3]), // y
    static_cast<int16_t>((buf[4] << 8) | buf[5])  // z
  };
  int16_t tempRaw = (buf[6] << 8) | buf[7];
  int16_t angVelRaw[3] = {
    static_cast<int16_t>((buf[8] << 8) | buf[9]),   // x
    static_cast<int16_t>((buf[10] << 8) | buf[11]), // y
    static_cast<int16_t>((buf[12] << 8) | buf[13])  // z
  };

  // Convert raw acceleration to G
  accel[0] = accelRaw[0] * ACCEL_CONVERSION[static_cast<size_t>(accelRange)];
  accel[1] = accelRaw[1] * ACCEL_CONVERSION[static_cast<size_t>(accelRange)];
  accel[2] = accelRaw[2] * ACCEL_CONVERSION[static_cast<size_t>(accelRange)];
  // Convert raw temperature to degrees (see p.30 of the register map document)
  temp = (tempRaw / 340.0f) + 36.53f;
  // Convert raw angular velocity to degrees per second
  angVel[0] = angVelRaw[0] * ANG_VEL_CONVERSION[static_cast<size_t>(gyroRange)];
  angVel[1] = angVelRaw[1] * ANG_VEL_CONVERSION[static_cast<size_t>(gyroRange)];
  angVel[2] = angVelRaw[2] * ANG_VEL_CONVERSION[static_cast<size_t>(gyroRange)];
}

void MPU6050::setAccelRange(AccelRange range)
{
  i2c->writeByteReg(devAddr, ACCEL_CONFIG, static_cast<uint8_t>(range) << 3);

  accelRange = range;
}

void MPU6050::setGyroRange(GyroRange range)
{
  i2c->writeByteReg(devAddr, GYRO_CONFIG, static_cast<uint8_t>(range) << 3);

  gyroRange = range;
}
