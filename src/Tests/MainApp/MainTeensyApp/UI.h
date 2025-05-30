#ifndef UI_H
#define UI_H

#include <Arduino.h>

class UI {
  private:
    const int buzzer = 33;
    
    const int leftButton = 6;
    const int rightButton = 12;

    bool lastLeftState = false;
    bool lastRightState = false;
  public:
    bool leftButtonState = false;
    bool rightButtonState = false;

    bool leftButtonToggle;
    bool rightButtonToggle;

    UI(){
      pinMode(leftButton, INPUT); pinMode(rightButton, INPUT); pinMode(buzzer, INPUT);
    };

    void update(){
      leftButtonState = digitalRead(leftButton);
      rightButtonState = digitalRead(rightButton);

      if (leftButtonState && !lastLeftState) {
        leftButtonToggle = !leftButtonToggle;
        buzz(800, 400);
      }

      if (rightButtonState && !lastRightState) {
        rightButtonToggle = !rightButtonToggle;
        buzz(800, 400);
      }

      lastLeftState = leftButtonState;
      lastRightState = rightButtonState;
    };

    void buzz(int freq, int time){
      tone(buzzer, freq, time);
    }
};

#endif