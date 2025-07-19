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
  PID pid;

  Robot(int motorsPWM)
    : pid(1, 0.12, motorsPWM){};

  unsigned long long updateTimer = 0;

  int ballAngle = 500;
  int ballIntensity = 0;
  int ballDistance = 1000;

  int lineAngle = 500;

  int blobX, blobY = -1;

  int bluetoothSignal = -1;

  float setpoint = 0;
  int correction = 0;
  bool firstDetected = false;
  int firstSector = -1;

  //int offset = 0;

  void updatePID() {
    if (ui.leftButtonState) {
      setpoint = imu.getYaw();
    }
    int offset = 0;

    float yaw = imu.getYaw();  //Serial.println(yaw);
    float error = yaw - setpoint + offset;
    //Serial.print(offset); Serial.print(" "); Serial.println(error);
    correction = pid.getCorrection(error);
  }

  int adjustBallAngleFar(int angle) {
    if (angle != 500) {
      if (angle > 45 && angle < 180) {
        return angle + 11;
      } else if (angle < 315 && angle >= 180) {
        return angle - 11;
      } else {
        return angle;
      }
    } else {
      return 500;
    }
  }

  int adjustBallAngleClose(int angle) {
    if (angle <= 360) {
      /*if(angle >= 0 && angle <= 135){
          return angle + 45;
        } else if(angle <= 360 && angle >= 225){
          return angle - 45;
        } else if(angle > 135 && angle <= 180){
          return angle + 90;
        } else if(angle < 225 && angle > 180){
          return angle - 90;
        } else return angle;*/
      if (angle >= 25 && angle < 180) {
        return angle + 45;
      } else if (angle >= 180 && angle <= 335) {
        return angle - 45;
      } else {
        return angle;
      }
    } else {
      return 500;
    }
  }

  int adjustLineAngle(int angle) {
    if (angle >= 0 && angle < 180) {
      return angle + 180;
    } else if (angle <= 360 && angle >= 180) {
      return angle - 180;
    } else {
      return angle;
    }
  }

  bool isBallOnFront() {
    //if (ballIntensity > 80 && (ballAngle < 35 || ballAngle > 340) && ballDistance < 800) return true;
    if (ballDistance < 95) return true;
    else return false;
  }

  bool lineDetected() {
    if (lineAngle <= 360) return true;
    else return false;
  }

  bool ballDetected() {
    if (ballAngle <= 360) return true;
    else return false;
  }

  int getLineSector(int lineAngle) {
    while (lineAngle < -15) lineAngle += 360;
    while (lineAngle >= 345) lineAngle -= 360;

    for (int i = 0; i < 12; i++) {
      int lower = -15 + (i * 30);
      int upper = 15 + (i * 30);
      if (lineAngle >= lower && lineAngle < upper) {
        return i;
      }
    }
    return -1;
  }

  int line_switch(int sector, int lastSector) {
    int angle = sector * 30;

    if (lastSector <= 3) {
      if (3 + lastSector <= sector && sector <= 8 + lastSector) {
        if (sector == 3) angle = 90;
        else angle = lastSector * 30;
      }
    } else if (4 <= lastSector && lastSector <= 8) {
      if (sector <= lastSector - 4 || lastSector + 3 <= sector) {
        angle = lastSector * 30;
      }
    } else if (9 <= lastSector) {
      if (lastSector - 9 <= sector && sector <= lastSector - 4) {
        angle = lastSector * 30;
      }
    }

    angle = (angle % 360 + 360) % 360;
    return angle;
  }

  bool hasBall() {
    static unsigned long ballSeenSince = 0;
    static bool tracking = false;

    bool currentBallState = (ballIntensity > 180 && ballDistance < 60);

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

  bool waitMillis(unsigned long duration) {
    static unsigned long startTime = 0;
    static bool waiting = false;

    if (!waiting) {
      startTime = millis();
      waiting = true;
      return false;  // Not yet finished
    }

    if (millis() - startTime >= duration) {
      waiting = false;
      return true;  // Done waiting
    }

    return false;  // Still waiting
  }

  bool goalDetected() {
    if (blobX < 500 && blobY < 250 && blobX > 0 && blobY > 0) return true;
    else return false;
  }

  float getCombinedAvoidanceAngle(int ballAngleDeg, int lineAngleDeg, float ballWeight = 0.6, float lineWeight = 0.4) {
    // Convert angles from degrees to radians
    float ballRad = radians(ballAngleDeg);
    float lineRad = radians(lineAngleDeg);

    // Calculate unit vectors
    float ballX = cos(ballRad);
    float ballY = sin(ballRad);

    float lineX = cos(lineRad);
    float lineY = sin(lineRad);

    // Combine vectors (line repulsion is negative)
    float combinedX = ballX * ballWeight + lineX * lineWeight;
    float combinedY = ballY * ballWeight + lineY * lineWeight;

    // Get the angle in degrees
    float angle = degrees(atan2(combinedY, combinedX));

    // Normalize angle to [0, 360)
    if (angle < 0) angle += 360;

    return angle;
  }
};

#endif