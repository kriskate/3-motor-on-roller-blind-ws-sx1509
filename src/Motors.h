#include <Arduino.h>

#include <SparkFunSX1509.h>
#include <Motor.h>

class Motors
{
private:
  void (*print)(String str);

  Motor motors[3];

  int min[3];
  int max[3];
  int current[3];
  int currentDir = 1; // -1
  bool autoMoving = false;

  void goUp(int motorID)
  {
    ccw ? motors[motorID].rotateCW() : motors[motorID].rotateCCW();
  }
  void goDown(int motorID)
  {
    ccw ? motors[motorID].rotateCCW() : motors[motorID].rotateCW();
  }

public:
  Motors() {}

  bool ccw = false;

  void setup(int motor_pins[3][4], SX1509 io, void (*print)(String str))
  {
    this->print = print;

    for (int i = 0; i < 3; ++i)
    {
      motors[i] = Motor(motor_pins[i], io, print);

      min[i] = 0;
      max[i] = 200000;
      current[i] = 0;
    }
  }

  // move motor checks
  void loop()
  {
    // if (sound) {  }
    // if websocket
    // if mqtt
    // if button

    if (millis() <= 5000)
      moveUp();
    else if (millis() <= 10000)
      moveDown();
  }

  void stop(int motorID = -1)
  {
    if (motorID == -1)
      for (int i = 0; i < 3; ++i)
        motors[i].stop();
    else
      motors[motorID].stop();
  }

  void moveDown(int motorID = 1, int steps = 1)
  {
    while (steps > 0)
    {
      steps--;
      if (motorID == -1)
        for (int i = 0; i < 3; ++i)
          goDown(i);
      else
        goDown(motorID);
    }
  }

  void moveUp(int motorID = -1, int steps = 1)
  {
    while (steps > 0)
    {
      steps--;
      if (motorID == -1)
        for (int i = 0; i < 3; ++i)
          goUp(i);
      else
        goUp(motorID);
    }
  }
};
