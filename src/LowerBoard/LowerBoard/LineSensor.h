#ifndef LINESENSOR_H
#define LINESENSOR_H

#define numSensors 18

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class LineSensor {
  public:
    LineSensor();
    void begin();
    void update();
    int getAngle();

  private:
    const int neoPin = 17;
    const int diodes[numSensors] = {A1, A0, A14, A15, A16, A17, A4, A5, A6, A9, A8, A7, A10, A11, A12, A13, A3, A2};
    Adafruit_NeoPixel pixels;
    unsigned long neoBeginInterval = 0;
    int readings[numSensors];
    const double vectorX[numSensors] = {
      1.0000000000,  0.9396926208,  0.7660444431,  0.5000000000,  0.1736481777,
      -0.1736481777, -0.5000000000, -0.7660444431, -0.9396926208, -1.0000000000,
      -0.9396926208, -0.7660444431, -0.5000000000, -0.1736481777,  0.1736481777,
      0.5000000000,  0.7660444431,  0.9396926208
    };
    const double vectorY[numSensors] = {
      0.0000000000,  0.3420201433,  0.6427876097,  0.8660254038,  0.9848077530,  
      0.9848077530,  0.8660254038,  0.6427876097,  0.3420201433,  0.0000000000, 
      -0.3420201433, -0.6427876097, -0.8660254038, -0.9848077530, -0.9848077530,  
      -0.8660254038, -0.6427876097, -0.3420201433
    };
    void calcVector();
    int angle = 0;
    int defaultGreenValue = 500;
    int greenValues[numSensors];
    bool calibrate = true;
};

#endif