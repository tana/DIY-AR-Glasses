#pragma once

#include "I2C.h"

// Driver for Honeywell HMC5833L compass
// Reference: https://www.digikey.jp/ja/datasheets/honeywellmicroelectronicsprecisionsensors/honeywell-microelectronics-precision-sensors-hmc5883l
class HMC5883L
{
public:
  enum class DataOutputRate : uint8_t
  {
    RATE_0_POINT_75_HZ = 0,
    RATE_1_POINT_5_HZ = 1,
    RATE_3_HZ = 2,
    RATE_7_POINT_5_HZ = 3,
    RATE_15_HZ = 4,
    RATE_30_HZ = 5,
    RATE_75_HZ = 6
  };

  enum class Gain : uint8_t
  {
    GAIN_1370 = 0,
    GAIN_1090 = 1,
    GAIN_820 = 2,
    GAIN_660 = 3,
    GAIN_440 = 4,
    GAIN_390 = 5,
    GAIN_330 = 6,
    GAIN_230 = 7
  };

  // Constructor
  HMC5883L(I2C* i2cBus, uint16_t addr = 0x1E);

  // Set data output rate in continuous measurement mode
  void setOutputRate(DataOutputRate rate);

  // Set gain
  void setGain(Gain gain);

  // Read measurement value and update "field" variable
  void read();

  float field[3]; // Magnetic field strength (in Gauss)

private:
  I2C* i2c;
  uint16_t devAddr;

  DataOutputRate outputRate;
  Gain gain;

  // Register addresses
  static const uint8_t CONF_REG_A = 0; // Configuration Register A
  static const uint8_t CONF_REG_B = 1; // Configuration Register B
  static const uint8_t MODE_REG = 2;   // Mode Register
  static const uint8_t DATA_OUT_X_MSB = 3; // Data Output X MSB
  static const uint8_t ID_REG_A = 10; // Identification Register A

  static constexpr int GAIN_VALUE[] = {
    1370, 1090, 820, 660, 440, 390, 330, 230
  };
};
