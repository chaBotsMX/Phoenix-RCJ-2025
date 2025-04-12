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

    for(int i = 0; i < numIR; i++){
      currReadings[i] = !digitalRead(ir[i]); //get current readings as booleans

      if(!prevReadings[i] && currReadings[i]){
        start[i] = micros(); //start counting time in HIGH
      }

      if(prevReadings[i] && !currReadings[i]){
        width[i] = micros() - start[i]; //stop counting time, save in array
        timeInZero[i] = millis() + 5;
      }

      if(!currReadings[i] && millis() > timeInZero[i]){
        width[i] = 0;
      }

      prevReadings[i] = currReadings[i];
    }
    calcVector();
    //adjustAngle();
  }
}

void IR::calcVector(){
  double sumX = 0;
  double sumY = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numIR; i++){
    if(width[i] > 0){
      sumX += width[i] * vectorX[i];
      sumY += width[i] * vectorY[i];
      sensorsReading++;
    }
  }
  
  angle = atan2(sumY, sumX) * (180.0 / M_PI);
  if(sensorsReading == 0) angle = 500;
  if(angle != 500) angle+=180;
  intensity = sqrt(pow(sumX, 2.0) + pow(sumY, 2.0));
  if(intensity > 2000) intensity = 2000;
}

void IR::adjustAngle(){
  int distance = intensity - maxIntensity;
  if (angle != 500) {
    if(angle < 180 && angle > 10){
      angle = angle + 90 * distance / maxIntensity;
    }
    else if(angle >= 180 && angle < 350){
      angle = angle - 90 * distance / maxIntensity;
    }
    else {
      angle = 0;
    }
  }
}

int IR::getAngle(){
  return angle;
}

int IR::getIntensity(){
  return intensity;
}

/*void IR::printIR(int angle, int intensity, unsigned long timeLimit, bool all=false){
  if((millis() - printUpdate) > timeLimit){
    printUpdate = millis();

    if(all){
      for(unsigned long w : width){
        Serial.print(w); Serial.print('\t');
      }
    }
    
    Serial.print(angle); Serial.print('\t');
    Serial.print(intensity); Serial.print('\n');
  }
}*/