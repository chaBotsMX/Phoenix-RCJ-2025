#include "Motors.h"

const int Motors::pwmPins[4] = {37, 25, 15, 10};
const int Motors::in1Pins[4] = {38, 26, 16, 11};
const int Motors::in2Pins[4] = {36, 24, 14, 9};

Motors::Motors(){
  for (int i = 0; i < 4; i++) {
    pinMode(pwmPins[i], OUTPUT);
    pinMode(in1Pins[i], OUTPUT);
    pinMode(in2Pins[i], OUTPUT);
  }
}

void Motors::driveToAngle(int angle, int power, int rotation){
    float A = -power * sin((angle + 45) * PI / 180.0) + rotation;
    float B = -power * cos((angle + 45) * PI / 180.0) + rotation;
    float C = power * sin((angle + 45) * PI / 180.0) + rotation;
    float D = power * cos((angle + 45) * PI / 180.0) + rotation;

    setOutput(0, A);
    setOutput(1, B);
    setOutput(2, C);
    setOutput(3, D);
}

void Motors::setOutput(int i, int power){
  power = constrain(power, -255, 255);

  analogWrite(pwmPins[i], abs(power));
  digitalWrite(in1Pins[i], power > 0);
  digitalWrite(in2Pins[i], power < 0);
}

void Motors::setAllMotorsOutput(int power){
  power = constrain(power, -255, 255);

  for(int i = 0; i < 4; i++){
    analogWrite(pwmPins[i], abs(power));
    digitalWrite(in1Pins[i], power > 0);
    digitalWrite(in2Pins[i], power < 0);
  }
}