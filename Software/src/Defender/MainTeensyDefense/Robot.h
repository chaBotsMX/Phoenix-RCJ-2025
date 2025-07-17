#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"
#include "Kicker.h"

class Robot {
  public:
    IMU imu;
    Motors motors;
    UART uart;
    UI ui;
    Kicker kicker;
    PID yawPID;
    PID linePID;

    Robot(int motorsPWM) : yawPID(1.5, 0.11, motorsPWM), linePID(10, 0.1, motorsPWM) {};

    unsigned long long updateTimer = 0;

    int ballAngle = 500;
    int ballIntensity = 0;
    int ballDistance = 1000;

    int lineAngle = 500;
    int lineDepth = 15;
    int lineSide = 0;

    int blobX = -1, blobArea = -1;

    int bluetoothSignal = -1;

    float setpoint = 0;
    int yawCorrection = 0;
    int lineCorrection = 0;
    bool firstDetected = false;
    int firstSector = -1;

    void updateYawPID(){
      if(ui.leftButtonState){
        setpoint = imu.getYaw();
      } 

      float yaw = imu.getYaw(); //Serial.println(yaw);
      float error = yaw - setpoint;
      yawCorrection = yawPID.getCorrection(error);
    }

    void updateLinePID(){
      int setpoint = 4;
      int error = setpoint - lineDepth;
      lineCorrection = linePID.getCorrection(error);
    }

    int adjustBallAngleFar(int angle){
      if(angle != 500){
        if(angle > 45 && angle < 180){
          return angle + 11;
        } else if(angle < 315 && angle >= 180){
          return angle - 11;
        } else{
          return angle;
        }
      } else{
        return 500;
      }
    }

    int adjustBallAngleClose(int angle){
      if(angle != 500){
        if(angle > 0 && angle < 135){
          return angle + 45;
        } else if(angle < 360 && angle >= 225){
          return angle - 45;
        } else if(angle > 135 && angle < 180){
          return angle + 90;
        } else if(angle < 225 && angle >= 180){
          return angle - 90;
        } else return angle;
      } else{
        return 500;
      }
    }

    int adjustLineAngle(int angle){
      if(angle >= 0 && angle < 180){
        return angle + 180;
      } else if(angle <= 360 && angle >= 180){
        return angle - 180;
      } else{
        return angle;
      }
    }

    bool isBallOnFront() {
      //if (ballIntensity > 80 && (ballAngle < 35 || ballAngle > 340) && ballDistance < 800) return true;
      if (ballDistance < 70) return true;
      else return false;
    }

    bool lineDetected(){
      if(lineAngle >= 0 && lineAngle <= 360) return true;
      else return false;
    }

    bool ballDetected(){
      if(ballAngle == 500) return false;
      else return true;
    }

    int getLineSector(int lineAngle) {
      while(lineAngle < -15) lineAngle += 360;
      while(lineAngle >= 345) lineAngle -= 360;

      for(int i = 0; i < 12; i++) {
        int lower = -15 + (i * 30);
        int upper = 15 + (i * 30);
        if(lineAngle >= lower && lineAngle < upper) {
          return i;
        }
      }
      return -1;
    }

    int line_switch(int sector, int lastSector) {
      int angle = sector * 30;

      if(lastSector <= 3) {
        if(3 + lastSector <= sector && sector <= 8 + lastSector) {
          if(sector == 3) angle = 90;
          else angle = lastSector * 30;
        }
      } else if(4 <= lastSector && lastSector <= 8) {
        if(sector <= lastSector - 4 || lastSector + 3 <= sector) {
          angle = lastSector * 30;
        }
      } else if(9 <= lastSector) {
        if(lastSector - 9 <= sector && sector <= lastSector - 4) {
          angle = lastSector * 30;
        }
      }

      angle = (angle % 360 + 360) % 360;
      return angle;
    }

    bool hasBall() {
      static unsigned long ballSeenSince = 0;
      static bool tracking = false;

      bool currentBallState = (ballIntensity > 180 && ballDistance < 50);

      if (currentBallState) {
        if (!tracking) {
          ballSeenSince = millis();
          tracking = true;
        }
        if (millis() - ballSeenSince >= 40) {
          return true;
        }
      } else {
        tracking = false;
        ballSeenSince = 0;
      }

      return false;
    }

    int getDefenseMovement(int lineAngleDeg, int ballAngleDeg, float k){
      //get components of lineAngle
      float lineAngleRad = radians(360 - lineAngleDeg);

      float correctionX = cos(lineAngleRad);
      float correctionY = sin(lineAngleRad);

      float directionRad = lineAngleRad + PI/2;

      int delta = (ballAngleDeg - lineAngleDeg + 360) % 360;
      if(delta > 180){
        directionRad = lineAngle - PI/2;
      }

      //get components of vector perpendicular to lineAngle (+90deg)
      float lineX = cos(directionRad);
      float lineY = sin(directionRad);

      float vectorX = correctionX + lineX * k;
      float vectorY = correctionY + lineY * k;

      float angle = degrees(atan2(vectorY, vectorX));
      if(angle < 0) angle+=360;
      Serial.println(angle);
      return angle;
    }

  bool isLineSideStable(int lineSide, unsigned long delayMs) {
    static unsigned long lastStartTime = 0;
    static bool lastStable = false;

    if (lineSide != 0) {
      if (lastStartTime == 0) {
        lastStartTime = millis();
      }

      if (millis() - lastStartTime >= delayMs) {
        lastStable = true;
      } else {
        lastStable = false;
      }
    } else {
      lastStartTime = 0;
      lastStable = false;
    }

    return lastStable;
  }

  bool goalDetected(){
    if(blobX < 250 && blobX > 0) return true;
    return false;
  }

  bool ballIsStable(int ballDistance, int ballIntensity){
    static unsigned long ballSeenSince = 0;
    static bool tracking = false;

    bool currentBallState = ((ballAngle <= 30 || ballAngle >= 330));

    if (currentBallState) {
      if (!tracking) {
        ballSeenSince = millis();
        tracking = true;
      }
      if (millis() - ballSeenSince >= 8000) {
        return true;
      }
    } else {
      tracking = false;
      ballSeenSince = 0;
    }

    return false;
  }
};

#endif