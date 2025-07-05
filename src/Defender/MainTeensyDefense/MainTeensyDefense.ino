/*---DEFENSE---*/

#include "Robot.h"

const int motorsPWM = 45;
const int basePow = 20;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& lineDepth = robot.lineDepth;
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
  lineDepth = robot.uart.getLineDepth(); Serial.print("depth: "); Serial.println(lineDepth);

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
        //int angle = 0;
        if(ballAngle >= 0 && ballAngle <= 180){
          //robot.motors.driveToAngle((lineAngle+90)%360, motorsPWM + abs(lineCorrection), yawCorrection);
          robot.motors.driveToAngle(90, motorsPWM + abs(lineCorrection), yawCorrection);
        } else{
          //robot.motors.driveToAngle((lineAngle+270)%360, motorsPWM + abs(lineCorrection), yawCorrection);
          robot.motors.driveToAngle(270, motorsPWM + abs(lineCorrection), yawCorrection);
        }
        Serial.print("correction: "); Serial.println(lineCorrection);
        break;
      }

      case STAY_IN_LINE:
        robot.motors.driveToAngle(lineAngle, basePow + abs(lineCorrection), yawCorrection);
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