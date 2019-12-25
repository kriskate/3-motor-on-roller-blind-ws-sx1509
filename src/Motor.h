#include <Arduino.h>
#include <SparkFunSX1509.h>

class Motor
{
private:
  int pins[4];
  SX1509 io;
  void (*print)(String str);

  int microStepsCount = 8;
  int microSteps[8][4] = {
      {HIGH, LOW, LOW, LOW},
      {HIGH, HIGH, LOW, LOW},
      {LOW, HIGH, LOW, LOW},
      {LOW, HIGH, HIGH, LOW},
      {LOW, LOW, HIGH, LOW},
      {LOW, LOW, HIGH, HIGH},
      {LOW, LOW, LOW, HIGH},
      {HIGH, LOW, LOW, HIGH},
  };
  int waveStepCount = 4;
  int waveSteps[4][4] = {
      {HIGH, LOW, LOW, LOW},
      {LOW, HIGH, LOW, LOW},
      {LOW, LOW, HIGH, LOW},
      {LOW, LOW, LOW, HIGH}};

  int fullStepCount = 4;
  int fullSteps[4][4] = {
      {HIGH, HIGH, LOW, LOW},
      {LOW, HIGH, HIGH, LOW},
      {LOW, LOW, HIGH, HIGH},
      {HIGH, LOW, LOW, HIGH}};

  int stepsPerRotation = 2048; //2049 steps per rotation when wave or full stepping
  int currentStep = 0;

  bool clockwise = true;

  int delayMS = 2; // 2 in original

  void step()
  {
    //Then we can figure out what our current step within the sequence from the overall current step
    //and the number of steps in the sequence
    int currentStepInSequence = currentStep % fullStepCount;
    currentStep++;

    //Figure out which step to use. If clock wise, it is the same is the current step
    //if not clockwise, we fire them in the reverse order...
    int directionStep = clockwise ? currentStepInSequence : (fullStepCount - 1) - currentStepInSequence;
    print(String(currentStep) + String(directionStep) + " dir step");

    //Set the four pins to their proper state for the current step in the sequence,
    //and for the current direction

    print(String(pins[0]) + " ---1--- " + String(millis()));
    for (int pin = 0; pin < fullStepCount; pin++)
      io.digitalWrite(pins[pin], fullSteps[directionStep][pin]);
    print(String(pins[0]) + " ---2--- " + String(millis()));

    // delayMicroseconds(400);
  }

public:
  Motor(){};
  Motor(int _pins[4], SX1509 &_io, void (*print)(String str))
  {
    this->print = print;
    io = _io;

    for (int i = 0; i < 4; i++)
    {
      pins[i] = _pins[i];
      io.pinMode(pins[i], OUTPUT);
      io.digitalWrite(pins[i], LOW);
    }
  }

  void stop()
  {
    currentStep = 0;

    for (int i = 0; i < 4; i++)
      io.digitalWrite(pins[i], LOW);
  }

  void rotateCCW()
  {
    if (clockwise)
      clockwise = false;
    step();
  }
  void rotateCW()
  {
    if (!clockwise)
      clockwise = true;
    step();
  }
};
