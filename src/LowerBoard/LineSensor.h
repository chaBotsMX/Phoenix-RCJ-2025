#ifndef LINESENSOR_H
#define LINESENSOR_H

#define numSensors 30

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LineSensor {
  public:
    LineSensor();
    void begin();
    void update();

  private:
    const int neoPin = 17;
    const int diodes[numSensors] = {35, 34, 37, 33, 18, 19, 20, 21, 2, 3, 4, 5, 28, 32, 31, 30, 36, 38, 39, 40, 41, 23, 22, 0, 1, 24, 25, 27, 26};
    Adafruit_NeoPixel pixels;
    unsigned long neoBeginInterval = 0;
    bool readings[numSensors];
};

#endif