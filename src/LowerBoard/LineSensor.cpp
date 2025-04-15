#include "LineSensor.h"

LineSensor::LineSensor()
  : pixels(numSensors, neoPin, NEO_GRB + NEO_KHZ800){
  for(int i = 0; i < numSensors; i++){
    pinMode(diodes[i], INPUT);
  }
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
}

void LineSensor::update(){
  for(int i = 0; i < numSensors; i++){
    readings[i] = digitalRead(diodes[i]);
  }
}