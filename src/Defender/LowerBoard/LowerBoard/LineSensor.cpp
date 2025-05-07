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
  calcVector();
}

void LineSensor::calcVector(){
  double sumX = 0;
  double sumY = 0;
  error = 0;
  int sensorsReading = 0;

  for(int i = 0; i < numSensors; i++){
    if(readings[i] < minGreenValue[i] - 100){
      sumX += vectorX[i];
      sumY += vectorY[i];
      sensorsReading++;
      error += posLateral[i];
    }
  }
  Serial.print(sensorsReading); Serial.println(" ");
  angle = atan2(sumX, sumY) * (180.0 / M_PI);
  if(sensorsReading == 0) angle = 500;
  else if(sensorsReading == 1) {angle = 0; error = 0;}
  Serial.print(angle); Serial.println(" ");
  Serial.print("posicion latera : "); Serial.println(error);
  //if(angle != 500) angle+=180;
}

void LineSensor::printLS(){
  for(int i = 0; i< numSensors; i++){
    Serial.print(readings[i]); Serial.print('\t');
  }
  Serial.println();
}
int LineSensor::calcShift(int ang) {
  int sgn   = (ang > 0) - (ang < 0);          // signo de ang
  float dif = ang - sgn * 90.0f;              // diferencia respecto a ±90
  int sh    = int( round(dif / 20.0f) );      // 1 paso = 20°
  if (sh >  5) sh =  5;
  if (sh < -5) sh = -5;
  return sh;
}
int LineSensor::getAngle(){
  return angle;
}