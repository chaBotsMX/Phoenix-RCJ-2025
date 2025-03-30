#include "IR.h"

IR::IR(){
  for (int i = 0; i < numIR; i++) {
    pinMode(ir[i], INPUT);
  }
}

void IR::update(){
  unsigned long lastUpdate = 0;

  if((micros() - lastUpdate) > 833){
    lastUpdate = micros();

    for(int i = 0; i < numIR; i++){
      currReadings[i] = !digitalRead(ir[i]); //get current readings as booleans

      if(!prevReadings[i] && currReadings[i]){
        start[i] = micros(); //start counting time in HIGH
      }

      if(prevReadings[i] && !currReadings[i]){
        intensity[i] = micros() - start[i]; //stop counting time, save in array
        timeInZero[i] = millis() + 5;
      }

      if(!currReadings[i] && millis() > timeInZero[i]){
        intensity[i] = 0;
      }

      prevReadings[i] = currReadings[i];
    }
    calcVector();
  }
}

void IR::calcVector(){
  double sumX = 0;
  double sumY = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numIR; i++){
    if(intensity[i] > 0){
      sumX += intensity[i] * vectorX[i];
      sumY += intensity[i] * vectorY[i];
      sensorsReading++;
    }
  }
  
  direction = atan2(sumY, sumX) * (180.0 / M_PI);
  if(sensorsReading == 0) direction = -1;
  magnitude = sqrt(pow(sumX, 2.0) + pow(sumY, 2.0));
}

int IR::getDirection(){
  return direction;
}

int IR::getMagnitude(){
  return magnitude;
}

void IR::printIR(){
  if((millis() - printUpdate) > 100){
    printUpdate = millis();
    /*for(unsigned long inst : intensity){
      Serial.print(inst);
      Serial.print('\t');
    }
    */
    Serial.print(direction); Serial.print('\t');
    Serial.print(magnitude); Serial.print('\n');
  }
}