/*---DEFENSE---*/

#include "Robot.h"

const int motorsPWM = 35;
const int basePWM = 20;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& lineDepth = robot.lineDepth;
auto& lineSide = robot.lineSide;
auto& yawCorrection = robot.yawCorrection;
auto& lineCorrection = robot.lineCorrection;

unsigned long lastTime;

enum RobotState{
  CATCH_BALL,
  STAY_IN_LINE,
  IDLE
};

RobotState currentState = IDLE;

RobotState determineState() {
  if (robot.lineDetected() && robot.ballDetected()) {
    return CATCH_BALL;
  } else if(robot.lineDetected()){
    return STAY_IN_LINE;
  }
  else return IDLE;
}

void setup() {
  Serial.begin(115200);
  robot.uart.beginIR(1000000);
  robot.uart.beginLine(1000000);

  delay(1000);

  if (!robot.imu.begin()) {
    Serial.println("imu not found");
    robot.ui.buzz(800, 400);
  }
}

void loop() {
  //unsigned long loopTime = micros() - lastTime;
  //lastTime = micros();
  robot.uart.receiveIRData();
  robot.uart.receiveLineData();

  ballAngle = robot.uart.getIRAngle(); //Serial.print(ballAngle); Serial.print('\t');
  ballIntensity = robot.uart.getIRIntensity(); //Serial.print(ballIntensity); Serial.print('\t');
  ballDistance = robot.uart.getIRDistance(); //Serial.println(ballDistance);

  lineAngle = robot.uart.getLineAngle(); //Serial.println(lineAngle);
  lineDepth = robot.uart.getLineDepth(); //Serial.print("depth: "); Serial.println(lineDepth);
  lineSide = robot.uart.getLineSide(); Serial.print("side: "); Serial.println(lineSide);

  if(millis() - robot.updateTimer >= 10){
    robot.updateTimer = millis();
    robot.ui.update();
    if(robot.imu.update()) robot.updateYawPID();
    if(robot.lineDetected()) robot.updateLinePID();
  }

  if(robot.ui.rightButtonToggle){
    currentState = determineState();

    switch(currentState){
      case CATCH_BALL: {
        if(lineSide == 0){
          if(lineDepth > 6){
            robot.motors.driveToAngle(0, basePWM + abs(lineCorrection), yawCorrection);
          } else if(lineDepth < 4){
            robot.motors.driveToAngle(180, basePWM + abs(lineCorrection), yawCorrection);
          } else{
            if(ballAngle >= 20 && ballAngle < 180){ 
              robot.motors.driveToAngle(90, motorsPWM, yawCorrection); 
            } else if(ballAngle <= 340 && ballAngle > 180){
              robot.motors.driveToAngle(270, motorsPWM, yawCorrection);
            } else{
              robot.motors.driveToAngle(0, 0, yawCorrection);
            }
          }
        } else{
          if(robot.isLineSideStable(lineSide, 900)){
            if(lineSide == 1){
              robot.motors.driveToAngle(90, motorsPWM * 1.2, yawCorrection);
            } else if(lineSide == 3){
              robot.motors.driveToAngle(270, motorsPWM * 1.2, yawCorrection);
            } else{
              robot.motors.driveToAngle(0, 0, yawCorrection);
            }
          } else{
            robot.motors.driveToAngle(0, 0, yawCorrection);
          }
        }
        break;
      }

      case STAY_IN_LINE:
        if(lineSide == 0){
          if(lineDepth > 5){
            robot.motors.driveToAngle(0, basePWM + abs(lineCorrection), yawCorrection);
          } else if(lineDepth < 5){
            robot.motors.driveToAngle(180, basePWM + abs(lineCorrection), yawCorrection);
          } else{
            robot.motors.driveToAngle(0, 0, yawCorrection);
          }
        } else{
          if(robot.isLineSideStable(lineSide, 500)){
            Serial.println("HEREEEEEEEEEEE");
            if(lineSide == 1){
              robot.motors.driveToAngle(90, motorsPWM * 1.2, yawCorrection);
            } else if(lineSide == 3){
              robot.motors.driveToAngle(270, motorsPWM * 1.2, yawCorrection);
            } else{
              robot.motors.driveToAngle(0, 0, yawCorrection);
            }
          }
        }
        break;

      case IDLE:
        robot.motors.driveToAngle(0, 0, yawCorrection);
        break;
    }
  } else{
    robot.motors.setAllMotorsOutput(0);
  }
  //Serial.println(loopTime);
}