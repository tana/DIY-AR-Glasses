#pragma once

#include <unordered_set>

enum class Eye
{
  LEFT,
  RIGHT
};

// Abstracted gamepad / keyboard input
enum class Operation
{
  LEFT,
  RIGHT,
  UP,
  DOWN,
  OK,
  CANCEL
};

class App
{
public:
  virtual ~App() = default;

  // Called once per frame
  virtual void update();

  // Called for each eye (twice per frame)
  virtual void draw(Eye eye) const;

protected:
  // Check whether op is started in this frame
  bool isOperationStarted(Operation op) const;

  // Check whether op is active
  bool isOperationActive(Operation op) const;

private:
  // Abstraction for gamepad and keyboard input
  // This is implemented for two reasons:
  //  1. IsGamepadButtonPressed did not work on my Pi, and
  //  2. to make it easy to test on PC.
  void processInput();

  std::unordered_set<Operation> operations, prevOperations;
};