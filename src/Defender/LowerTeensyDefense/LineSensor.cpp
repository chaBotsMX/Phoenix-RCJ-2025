#include "LineSensor.h"

LineSensor::LineSensor()
  : pixels(numSensors, neoPin, NEO_GRB + NEO_KHZ800){
}

void LineSensor::begin(){
  pixels.begin();
  pixels.clear();

  for(int i = 0; i < numSensors; i++){
    unsigned long start = millis();
    while(millis() - start < 50){}
    pixels.setPixelColor(i, pixels.Color(250, 0, 0));
    pixels.show();
  }

  unsigned long start = millis();
  while(millis() - start < 100){}

  if(calibrate){
    Serial.print("green val ");
    for(int i = 0; i < numSensors; i++){
      minGreenValue[i] = 1023;
      //greenValues[i] = analogRead(diodes[i]); //Serial.print(greenValues[i]); Serial.print('\t');
      for(int j = 0; j < 20; j++){
        if(analogRead(diodes[i]) < minGreenValue[i]){
          minGreenValue[i] = analogRead(diodes[i]);
        }
      }
      Serial.print(minGreenValue[i]); Serial.print('\t');
    }
    Serial.println();
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

  calculateLineVector();
  calculateDepth();
}

void LineSensor::calculateLineVector(){
  double sumX = 0;
  double sumY = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numSensors; i++){
    if(readings[i] < minGreenValue[i] - 55){
      sumX += vectorX[i];
      sumY += vectorY[i];
      sensorsReading++;
      detectedSensors[i] = true;
    } else detectedSensors[i] = false;
  }

  if(sensorsReading == 0){
    angle = 500; depth = 15;
  }
  else angle = (atan2(sumY, sumX) * (180.0 / M_PI)) + 180;
}

void LineSensor::calculateDepth(){
  int sensorPair = 17;
  for(int i = 0; i < numSensors / 2; i++){
    if(detectedSensors[14]){
      depth = 6;
      break;
    }
    if(detectedSensors[i] && detectedSensors[sensorPair]){
      depth = abs(i-9);
      break;
    } else sensorPair--;
  }
}

void LineSensor::printLS(){
  Serial.print("Detected sensors: ");
  for(int i = 0; i< numSensors; i++){
    //Serial.print(readings[i]); Serial.print('\t');
    Serial.print(detectedSensors[i]); Serial.print('\t');
  }
  Serial.println();
}

int LineSensor::getAngle(){
  return angle/2;
}

int LineSensor::getDepth(){
  return depth;
}