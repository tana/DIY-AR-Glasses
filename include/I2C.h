#pragma once

#include <cstddef>
#include <cstdint>

// I2C bus (wraps I2C userspace API of Linux)
// References:
//  https://www.kernel.org/doc/html/latest/i2c/dev-interface.html
//  https://www.nds-osk.co.jp/download-files/forum/freedownload/06/casestudy1/UserlandIO.pdf
class I2C
{
public:
  // Constructor
  I2C(int adapterNum);
  // Prohibit copy
  I2C(const I2C&) = delete;
  I2C& operator=(const I2C&) = delete;

  // Destructor
  ~I2C();

  // Read bytes from a register on an I2C device
  void readReg(uint16_t devAddr, uint8_t regAddr, uint8_t* buf, size_t len);

  // Read a byte from a register on an I2C device
  // (wrapper of readReg for convenience)
  uint8_t readByteReg(uint16_t devAddr, uint8_t regAddr);

  // Write bytes to a register on an I2C device
  void writeReg(uint16_t devAddr, uint8_t regAddr, uint8_t* buf, size_t len);

  // Write a byte to a register on an I2C device
  // (wrapper of writeReg for convenience)
  void writeByteReg(uint16_t devAddr, uint8_t regAddr, uint8_t value);

private:
  int fd;
};

