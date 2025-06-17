#ifndef KICKER_H
#define KICKER_H

#include <Arduino.h>

class Kicker {
  private:
    const int solenoid = 32;
    unsigned long intervalTimer = 0;
    unsigned long kickingTimer = 0;
    bool isActive = false;
  public:
    Kicker(){
      pinMode(solenoid, OUTPUT);
    };

    /*void kick(){
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
    }*/

    void kick(){
      unsigned long now = millis();

      if(!isActive && now - intervalTimer >= 5000){
        digitalWrite(solenoid, HIGH);
        intervalTimer = now;
        kickingTimer = now;
        isActive = true;
      }

      if(isActive && now - kickingTimer >= 10){
        digitalWrite(solenoid, LOW);
        isActive = false;
      }
    }
};

#endif