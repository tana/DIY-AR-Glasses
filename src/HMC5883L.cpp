#include "HMC5883L.h"

#include <stdexcept>

HMC5883L::HMC5883L(I2C* i2cBus, uint16_t addr)
  : i2c(i2cBus), devAddr(addr)
{
  // Check if the host can properly communicate with the HMC5883L
  uint8_t idBuf[3];
  i2c->readReg(devAddr, ID_REG_A, idBuf, 3);
  if (idBuf[0] != 0b01001000 || idBuf[1] != 0b00110100 || idBuf[2] != 0b00110011) {
    throw std::runtime_error("Identification Registers have wrong value");
  }

  // Average 1 sample, data output rate 15 Hz, normal measurement configuration
  i2c->writeByteReg(devAddr, CONF_REG_A, 0b00010000);
  outputRate = DataOutputRate::RATE_15_HZ;

  setGain(Gain::GAIN_1090);

  // Continuous measurement mode
  i2c->writeByteReg(devAddr, MODE_REG, 0b00000000);
}

void HMC5883L::setOutputRate(DataOutputRate rate)
{
  uint8_t oldVal = i2c->readByteReg(devAddr, CONF_REG_A);
  i2c->writeByteReg(devAddr, CONF_REG_A, (oldVal & (~0b00011100)) | (static_cast<uint8_t>(rate) << 2));

  outputRate = rate;
}

void HMC5883L::setGain(Gain gain)
{
  i2c->writeByteReg(devAddr, CONF_REG_B, static_cast<uint8_t>(gain) << 5);

  this->gain = gain;
}

void HMC5883L::read()
{
  uint8_t buf[6];
  // Read values of 3 axes at once
  i2c->readReg(devAddr, DATA_OUT_X_MSB, buf, 6);

  // Convert from big endian
  // Note: Output rgisters inside HMC5883L are x,z,y order
  int16_t fieldRaw[3] = {
    static_cast<int16_t>((buf[0] << 8) | buf[1]), // x
    static_cast<int16_t>((buf[4] << 8) | buf[5]), // y
    static_cast<int16_t>((buf[2] << 8) | buf[3])  // z
  };

  // Convert to Gauss
  field[0] = float(fieldRaw[0]) / GAIN_VALUE[static_cast<uint8_t>(gain)];
  field[1] = float(fieldRaw[1]) / GAIN_VALUE[static_cast<uint8_t>(gain)];
  field[2] = float(fieldRaw[2]) / GAIN_VALUE[static_cast<uint8_t>(gain)];
}
