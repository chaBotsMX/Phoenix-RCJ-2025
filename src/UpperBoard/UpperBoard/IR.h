#ifndef IR_H
#define IR_H

#define numIR 18

#include <Arduino.h>

class IR {
  public:
    IR();
    void update();
    void printIR();
    int getDirection();
    int getMagnitude();
  private:
    const int ir[numIR] = {33, 34, 36, 37, 38, 19, 20, 22, 23, 0, 1, 3, 4, 27, 28, 29, 31, 32};
    bool currReadings[numIR];
    bool prevReadings[numIR];
    unsigned long intensity[numIR];
    unsigned long start[numIR];
    unsigned long timeInZero[numIR];
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
    double direction = -1;
    int magnitude = 0;
    void calcVector();
    unsigned long printUpdate = 0;
    const double maxWidth = 600.0;
};

#endif