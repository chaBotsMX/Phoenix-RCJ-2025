#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>

class Motors{
  public:
    Motors();
    void driveToAngle(int angle, int power, int rotation);
    void setOutput(int i, int power);
    void setAllMotorsOutput(int power);
  private:
    static const int pwmPins[4];
    static const int in1Pins[4];
    static const int in2Pins[4];
    float scale = 0;
};

#endif