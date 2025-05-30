#ifndef KICKER_H
#define KICKER_H

#include <Arduino.h>

class Kicker {
  private:
    const int solenoid = 32;
    unsigned long timer;
    bool isActive = false;
  public:
    Kicker(){
      pinMode(solenoid, OUTPUT);
    };

    void kick(){
      if(!isActive){
        digitalWrite(solenoid, HIGH);
        timer = millis();
        isActive = true;
      }
    }

    void update(){
      if(isActive && (millis() - timer >= 10)){
        digitalWrite(solenoid, LOW);
        isActive = false;
      }
    }
};

#endif