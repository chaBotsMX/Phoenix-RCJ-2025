#ifndef UI_H
#define UI_H

#include <Arduino.h>

class UI {
  private:
    const int leftButton = 6;
    const int rightButton = 12;
    bool leftState;
    bool rightState;
  public:
    bool leftButtonWasPressed;
    bool rightButtonWasPressed;

    UI(){
      pinMode(leftButton, INPUT); pinMode(rightButton, INPUT);
    };

    void update(){
      leftState = digitalRead(leftButton);
      rightState = digitalRead(rightButton);

      if(leftState && !leftButtonWasPressed){
        leftButtonWasPressed = !leftButtonWasPressed;
      }

      if(rightState && !rightButtonWasPressed){
        rightButtonWasPressed = !rightButtonWasPressed;
      }
    };
};

#endif