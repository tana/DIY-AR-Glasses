#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>
#include <signal.h>

#include "raylib-cpp.hpp"
#include <fmt/core.h>

#include "I2C.h"
#include "HMC5883L.h"

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
  HMC5883L mag(&i2c);
  mag.setOutputRate(HMC5883L::DataOutputRate::RATE_75_HZ);

  // Handle Ctrl+C
  struct sigaction action;
  action.sa_handler = handleInterrupt;
  if (sigaction(SIGINT, &action, nullptr) != 0) {
    std::cerr << "Cannot register signal handler" << std::endl;
    return -1;
  }

  std::cout << "Press Ctrl+C to stop" << std::endl;

  std::vector<raylib::Vector3> measurements; 

  // Collect data
  while (!shouldStop) {
    mag.read();
    measurements.emplace_back(mag.field[0], mag.field[1], mag.field[2]);

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  // Calculate offset
  const auto sum = std::accumulate(measurements.begin(), measurements.end(), raylib::Vector3(0, 0, 0));
  const auto offset = sum / measurements.size();

  fmt::print("Offset = ({}, {}, {})\n", offset.x, offset.y, offset.z);

  return 0;
}
