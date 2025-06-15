#ifndef IR_H
#define IR_H

#define numIR 18
#define photodiode A15
#define alpha 0.05
#define maxIntensity 2200

#include <Arduino.h>

class IR {
  public:
    IR();
    void update(unsigned long timeLimit);
    void printIR(int angle, int intensity, unsigned long timeLimit, bool all=false);

    int getAngle();
    int getIntensity();
    int getDistance();

  private:
    //const int ir[numIR] = {33, 34, 36,  37, 38, 15, 20, 22, 23, 0, 1, 3, 4, 27, 28, 29, 31, 32};
    const int ir[numIR] = {0, 1, 3, 4, 27, 28, 29, 31, 32, 33, 34, 36,  37, 38, 15, 20, 22, 23};

    const float vectorX[numIR] = {
      1.0000000000,  0.9396926208,  0.7660444431,  0.5000000000,  0.1736481777,
      -0.1736481777, -0.5000000000, -0.7660444431, -0.9396926208, -1.0000000000,
      -0.9396926208, -0.7660444431, -0.5000000000, -0.1736481777,  0.1736481777,
      0.5000000000,  0.7660444431,  0.9396926208
    };
    const float vectorY[numIR] = {
      0.0000000000,  0.3420201433,  0.6427876097,  0.8660254038,  0.9848077530,  
      0.9848077530,  0.8660254038,  0.6427876097,  0.3420201433,  0.0000000000, 
      -0.3420201433, -0.6427876097, -0.8660254038, -0.9848077530, -0.9848077530,  
      -0.8660254038, -0.6427876097, -0.3420201433
    };
    
    int rawAngle = -1;
    int intensity = 0;
    int photodiodeDistance = 1000;

    bool detected[numIR], lastDetected[numIR];
    unsigned long pulseWidth[numIR], timeInHigh[numIR], timeInLow[numIR];
    
    void updateTSSP();
    void updatePhotodiodes();
    void calculateBallVector();
};

#endif