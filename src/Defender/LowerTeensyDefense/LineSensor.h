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
    void printLS();
    int getAngle();
    int getDepth();

  private:
    const int neoPin = 33;
    //const int diodes[numSensors] = {A1, A0, A14, A15, A16, A17, A4, A5, A6, A9, A8, A7, A10, A11, A12, A13, A3, A2};
    const int diodes[numSensors] = {A9, A8, A7, A10, A11, A12, A13, A3, A2, A1, A0, A14, A15, A16, A17, A4, A5, A6};
    Adafruit_NeoPixel pixels;
    unsigned long neoBeginInterval = 0;
    int readings[numSensors];
    const float vectorX[numSensors] = {
      0.9848,  0.8660,  0.6428,  0.3420,  0.0000, -0.3420,
      -0.6428, -0.8660, -0.9848, -0.9848, -0.8660, -0.6428,
      -0.3420, -0.0000,  0.3420,  0.6428,  0.8660,  0.9848
    };
    const float vectorY[numSensors] = {
      0.1736,  0.5000,  0.7660,  0.9397,  1.0000,  0.9397,
      0.7660,  0.5000,  0.1736, -0.1736, -0.5000, -0.7660,
      -0.9397, -1.0000, -0.9397, -0.7660, -0.5000, -0.1736
    };
    bool detectedSensors[numSensors];
    void calculateLineVector();
    int angle = 0;
    int defaultGreenValue = 950;
    int greenValues[numSensors];
    bool calibrate = true;
    int minGreenValue[numSensors];
    int depth = 15;
    void calculateDepth();
};

#endif