#include "LineSensor.h"

LineSensor::LineSensor()
  : pixels(numSensors, neoPin, NEO_GRB + NEO_KHZ800){
}

void LineSensor::begin(){
  pixels.begin();
  pixels.clear();

  for(int i = 0; i < numSensors; i++){
    if(millis() > neoBeginInterval){
      neoBeginInterval = millis() + 100;
      pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      pixels.show();
    }
  }

  if(calibrate){
    for(int i = 0; i < numSensors; i++){
      greenValues[i] = analogRead(diodes[i]) + 30;
    }
  } else{
    for(int i = 0; i < numSensors; i++){
      greenValues[i] = defaultGreenValue;
    }
  }
}

void LineSensor::update(){
  for(int i = 0; i < numSensors; i++){
    readings[i] = analogRead(diodes[i]);
  }
  calcVector();
}

void LineSensor::calcVector(){
  double sumX = 0;
  double sumY = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numSensors; i++){
    if(readings[i] > greenValues[i]){
      sumX += vectorX[i];
      sumY += vectorY[i];
      sensorsReading++;
    }
  }

  angle = atan2(sumY, sumX) * (180.0 / M_PI);
  if(sensorsReading == 0) angle = 500;
  if(angle != 500) angle+=180;
}

int LineSensor::getAngle(){
  return angle;
}