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
      currReadings[i] = !digitalReadFast(ir[i]); //get current readings as booleans

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
    adjustAngle();
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
  
  rawAngle = atan2(sumY, sumX) * (180.0 / M_PI);
  if(sensorsReading == 0) rawAngle = 500;
  if(rawAngle != 500) rawAngle += 180;

  intensity = sqrt(pow(sumX, 2.0) + pow(sumY, 2.0));
  if(intensity > maxIntensity) intensity = maxIntensity;
  distance = maxIntensity - intensity;
}

void IR::adjustAngle(){
  if(rawAngle != 500){
    double x = 0, y = 0;
    distance = constrain(distance,300,100000);
    x = distance * cos(rawAngle * PI/180);
    y = distance * sin(rawAngle * PI/180);

    angle = atan2(y, x - 265) * 180 / PI;
    angle += 360;
    angle > 360 ? angle -= 360 : angle = angle;
  } else{
    angle = rawAngle;
  }
}

/*
void IR::adjustAngle(){
  if (angle != 500) {
    if(angle < 180 && angle > 20){ //right
      if(intensity > maxIntensityR) intensity = maxIntensityR;
      intensity = map(intensity, 0, maxIntensityR, 0, 100);
      distance = intensity - 100;
      angle +=  90 * 1 - (distance / 100);
    }
    else if(angle >= 180 && angle < 340){ //left
      if(intensity > maxIntensityL) intensity = maxIntensityL;
      intensity = map(intensity, 0, maxIntensityL, 0, 100);
      distance = intensity - 100;
      angle -= 90 * 1 - (distance / 100);
    }
    else {
      angle = 0;
    }
    if(angle < 0) angle = 0;
    if(angle > 360) angle = 360;
  }
}*/

int IR::getRawAngle(){
  return rawAngle;
}

int IR::getAngle(){
  return angle;
}

int IR::getIntensity(){
  return intensity;
}

void IR::printIR(int angle, int intensity, unsigned long timeLimit, bool all=false){
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
}