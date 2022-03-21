#include "I2C.h"

#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

#include <fmt/core.h>

I2C::I2C(int adapterNum)
{
  // Open an I2C adapter
  fd = open(fmt::format("/dev/i2c-{}", adapterNum).c_str(), O_RDWR);
  if (fd < 0) {
    throw std::runtime_error(strerror(errno));
  }
}

I2C::~I2C()
{
  // Close the I2C adapter device (/dev/i2c-N)
  close(fd);
}

void I2C::readReg(uint16_t devAddr, uint8_t regAddr, uint8_t* buf, size_t len)
{
  // Define transaction
  struct i2c_msg msgs[2] = {
    // Send register address
    {
      .addr = devAddr,
      .flags = 0, // 0 indicates write (See page 17 of https://www.nds-osk.co.jp/download-files/forum/freedownload/06/casestudy1/UserlandIO.pdf )
      .len = 1, // Register address is 1 byte
      .buf = &regAddr
    },
    // Receive content
    {
      .addr = devAddr,
      .flags = I2C_M_RD,  // I2C_M_RD indicates read
      .len = static_cast<uint16_t>(std::min(len, UINT32_MAX)),
      .buf = buf
    }
  };

  // Execute the above defined transaction
  struct i2c_rdwr_ioctl_data rdwrData{ .msgs = msgs, .nmsgs = 2 };
  if (ioctl(fd, I2C_RDWR, &rdwrData) < 0) {
    throw std::runtime_error(strerror(errno));
  }
}

uint8_t I2C::readByteReg(uint16_t devAddr, uint8_t regAddr)
{
  uint8_t val;
  
  readReg(devAddr, regAddr, &val, 1);
  return val;
}

void I2C::writeReg(uint16_t devAddr, uint8_t regAddr, uint8_t* buf, size_t len)
{
  // Concatenate register address and data into single array
  // (Because separate i2c_msg didn't work well)
  std::vector<uint8_t> sendBuf(1 + len);
  sendBuf[0] = regAddr;
  for (int i = 0; i < len; ++i) {
    sendBuf[1 + i] = buf[i];
  }

  // Define transaction
  struct i2c_msg msgs[1] = {
    // Send register address and content
    {
      .addr = devAddr,
      .flags = 0, // 0 indicates write (See page 17 of https://www.nds-osk.co.jp/download-files/forum/freedownload/06/casestudy1/UserlandIO.pdf )
      .len = static_cast<uint16_t>(std::min(sendBuf.size(), UINT32_MAX)),
      .buf = sendBuf.data()
    }
  };

  // Execute the above defined transaction
  struct i2c_rdwr_ioctl_data rdwrData{ .msgs = msgs, .nmsgs = 1 };
  if (ioctl(fd, I2C_RDWR, &rdwrData) < 0) {
    throw std::runtime_error(strerror(errno));
  }
}

void I2C::writeByteReg(uint16_t devAddr, uint8_t regAddr, uint8_t value)
{
  writeReg(devAddr, regAddr, &value, 1);
}
