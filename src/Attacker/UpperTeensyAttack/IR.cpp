#include "IR.h"

IR::IR(){
  for (int i = 0; i < numIR; i++) {
    pinMode(ir[i], INPUT);
  }
}

void IR::update(unsigned long timeLimit){
  unsigned long lastUpdate = 0;

  if((micros() - lastUpdate) > timeLimit){
    lastUpdate = micros();

    updateTSSP();
    updatePhotodiodes();
    calculateBallVector();
  }
}

void IR::updateTSSP(){
  unsigned long currentMicros = micros();
  unsigned long currentMillis = millis();

  for(int i = 0; i < numIR; i++){
    detected[i] = !digitalReadFast(ir[i]); //get current readings as booleans

    if(!lastDetected[i] && detected[i]){
      timeInHigh[i] = currentMicros; //start counting time in HIGH
    }
    if(lastDetected[i] && !detected[i]){
      pulseWidth[i] = currentMicros - timeInHigh[i]; //stop counting time, save in array
      timeInLow[i] = currentMillis + 5;
    }

    if(!lastDetected[i] && currentMillis > timeInLow[i]){
      pulseWidth[i] = 0;
    }

    lastDetected[i] = detected[i];
  }
}

void IR::updatePhotodiodes(){
  int photoReadings = analogRead(photodiode);
  photodiodeDistance = alpha * photoReadings + (1 - alpha) * photodiodeDistance;
}

void IR::calculateBallVector(){
  float sumX = 0, sumY = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numIR; i++){
    if(pulseWidth[i] > 0){
      sumX += pulseWidth[i] * vectorX[i];
      sumY += pulseWidth[i] * vectorY[i];
      sensorsReading++;
    }
  }
  
  if(sensorsReading == 0) rawAngle = 500;
  else rawAngle = (atan2(sumY, sumX) * (180.0 / M_PI)) + 180;

  intensity = sqrt((sumX * sumX) + (sumY * sumY));
  if(intensity > maxIntensity) intensity = maxIntensity;
}

int IR::getAngle(){
  return rawAngle/2;
}

int IR::getIntensity(){
  return map(intensity, 0, 2000, 0, 100);
}

int IR::getDistance(){
  return photodiodeDistance/10;
}

void IR::printIR(int angle, int intensity, unsigned long timeLimit, bool all=false){
  unsigned long printUpdate = 0;
  if((millis() - printUpdate) > timeLimit){
    printUpdate = millis();

    if(all){
      for(unsigned long w : pulseWidth){
        Serial.print(w); Serial.print('\t');
      }
    }
    
    Serial.print(angle); Serial.print('\t');
    Serial.print(intensity); Serial.print('\n');
  }
}