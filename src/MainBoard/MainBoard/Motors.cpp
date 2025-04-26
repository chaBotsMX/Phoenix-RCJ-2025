#include "Motors.h"

const int Motors::pwmPins[4] = {37, 25, 15, 10};
const int Motors::in1Pins[4] = {38, 26, 16, 11};
const int Motors::in2Pins[4] = {36, 24, 14, 9};

Motors::Motors(){
  for (int i = 0; i < 4; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWriteFrequency(pwmPins[i], 20000);
    pinMode(in1Pins[i], OUTPUT);
    pinMode(in2Pins[i], OUTPUT);
  }
}

void Motors::driveToAngle(int angle, int power, int rotation){
  float B = -power * sin((-angle + 45) * PI / 180.0);
  float D = -power * cos((-angle + 45) * PI / 180.0);
  float C = power * sin((-angle + 45) * PI / 180.0);
  float A = power * cos((-angle + 45) * PI / 180.0);

  Serial.print(A); Serial.print('\t');
  Serial.print(B); Serial.print('\t');
  Serial.print(C); Serial.print('\t');
  Serial.print(D); Serial.print('\t');

  float maxVal = max(max(abs(A), abs(B)), max(abs(C), abs(D))); Serial.print(maxVal); Serial.print('\t');
  if (maxVal > 0) {
    scale = float(power) / maxVal;
    Serial.print(scale); Serial.print('\n');
    A = A * scale;
    B = B * scale;
    C = C * scale;
    D = D * scale;

    /*Serial.print(A); Serial.print('\t');
    Serial.print(B); Serial.print('\t');
    Serial.print(C); Serial.print('\t');
    Serial.print(D); Serial.print('\t');
    Serial.println(scale);*/
  }

  setOutput(0, A + rotation); //back right
  setOutput(1, B + rotation); //front right
  setOutput(2, C + rotation); //front left
  setOutput(3, D + rotation); //back left
}

void Motors::setOutput(int i, int power){
  power = constrain(power, -255, 255);

  analogWrite(pwmPins[i], abs(power));
  digitalWrite(in2Pins[i], power > 0);
  digitalWrite(in1Pins[i], power < 0);
}

void Motors::setAllMotorsOutput(int power){
  power = constrain(power, -255, 255);

  for(int i = 0; i < 4; i++){
    analogWrite(pwmPins[i], abs(power));
    digitalWrite(in2Pins[i], power > 0);
    digitalWrite(in1Pins[i], power < 0);
  }
}