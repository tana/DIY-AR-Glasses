#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>
#include <signal.h>

#include "raylib-cpp.hpp"
#include <fmt/core.h>

#include "I2C.h"
#include "MPU6050.h"

const int I2C_BUS_NUM = 1;  // /dev/i2c-1

bool shouldStop = false;

// Called when Ctrl+C is pressed
void handleInterrupt(int signum)
{
  shouldStop = true;
}

int main()
{
  I2C i2c(I2C_BUS_NUM);
  MPU6050 imu(&i2c);

  // Handle Ctrl+C
  struct sigaction action;
  action.sa_handler = handleInterrupt;
  if (sigaction(SIGINT, &action, nullptr) != 0) {
    std::cerr << "Cannot register signal handler" << std::endl;
    return -1;
  }

  std::cout << "Put the MPU6050 in z-up position" << std::endl;
  std::cout << "Press Ctrl+C to stop" << std::endl;

  std::vector<raylib::Vector3> measurements; 

  // Collect data
  while (!shouldStop) {
    imu.read();
    measurements.emplace_back(imu.accel[0], imu.accel[1], imu.accel[2]);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // Calculate offset
  // TODO: compensate device tilt
  const auto sum = std::accumulate(measurements.begin(), measurements.end(), raylib::Vector3(0, 0, 0));
  const auto offset = sum / measurements.size() - raylib::Vector3(0, 0, 1);  // Subtract gravitational acceleration

  fmt::print("Offset = ({}, {}, {})\n", offset.x, offset.y, offset.z);

  return 0;
}

