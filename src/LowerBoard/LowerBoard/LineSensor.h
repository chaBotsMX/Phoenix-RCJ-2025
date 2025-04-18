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
    const int diodes[numSensors] = {35, 34, 37, 33, 18, 19, 20, 21, 2, 3, 4, 5, 28, 32, 31, 30, 36, 38};
    Adafruit_NeoPixel pixels;
    unsigned long neoBeginInterval = 0;
    bool readings[numSensors];
    const double vectorX[numIR] = {
      1.0000000000,  0.9396926208,  0.7660444431,  0.5000000000,  0.1736481777,
      -0.1736481777, -0.5000000000, -0.7660444431, -0.9396926208, -1.0000000000,
      -0.9396926208, -0.7660444431, -0.5000000000, -0.1736481777,  0.1736481777,
      0.5000000000,  0.7660444431,  0.9396926208
    };
    const double vectorY[numIR] = {
      0.0000000000,  0.3420201433,  0.6427876097,  0.8660254038,  0.9848077530,  
      0.9848077530,  0.8660254038,  0.6427876097,  0.3420201433,  0.0000000000, 
      -0.3420201433, -0.6427876097, -0.8660254038, -0.9848077530, -0.9848077530,  
      -0.8660254038, -0.6427876097, -0.3420201433
    };
    void calcVector();
    int angle = 0;
};

#endif