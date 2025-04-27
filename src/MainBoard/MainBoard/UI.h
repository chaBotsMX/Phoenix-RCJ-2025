#ifndef UI_H
#define UI_H

#include <Arduino.h>

class UI {
  private:
    const int buzzer = 33;
    
    const int leftButton = 6;
    const int rightButton = 12;

    bool leftButtonState = false;
    bool rightButtonState = false;
    bool lastLeftState = false;
    bool lastRightState = false;
  public:
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
      }

      if (rightButtonState && !lastRightState) {
        rightButtonToggle = !rightButtonToggle;
        tone(buzzer, 800, 400);
      }

      lastLeftState = leftButtonState;
      lastRightState = rightButtonState;
    };
};

#endif