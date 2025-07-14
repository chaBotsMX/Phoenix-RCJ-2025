/*---ATTACK---*/

#include "Robot.h"

const int motorsPWM = 60;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& correction = robot.correction;

unsigned long lastTime;

enum RobotState{
  AVOID_LINE,
  HAS_BALL,
  BALL_CLOSE,
  BALL_FAR,
  IDLE
};

RobotState currentState = IDLE;

RobotState determineState() {
  if (robot.lineDetected()) {
    return AVOID_LINE;
  } 
  else if (robot.isBallOnFront()) {
    return HAS_BALL;
  }
  else if (robot.ballDetected()) {
    if(ballIntensity < 200) return BALL_FAR;
    return BALL_CLOSE;
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

  ballAngle = robot.uart.getIRAngle();
  ballIntensity = robot.uart.getIRIntensity();
  ballDistance = robot.uart.getIRDistance();

  lineAngle = robot.uart.getLineAngle();

  if(millis() - robot.updateTimer >= 10){
    robot.updateTimer = millis();
    Serial.print("ballAngle: "); Serial.println(ballAngle);
    Serial.print("ballIntensity: "); Serial.println(ballIntensity);
    Serial.print("adjustBallAngleClose: "); Serial.println(robot.adjustBallAngleClose(ballAngle));
    robot.ui.update();
    if(robot.imu.update()) robot.updatePID();
  }

  if(robot.ui.rightButtonToggle){
    currentState = determineState();

    switch(currentState){
      case AVOID_LINE: {
        Serial.println("state avoid line");
        robot.ui.buzz(400, 400);

        if(!robot.firstDetected){
          robot.firstSector = robot.getLineSector(lineAngle);
          robot.firstDetected = true;
        }
        int sector = robot.getLineSector(lineAngle);
        int avoidAngle = robot.adjustLineAngle(robot.line_switch(sector, robot.firstSector));

        robot.motors.driveToAngle(avoidAngle, motorsPWM * 1.2, correction);
        break;
      }
      
      case HAS_BALL:
        Serial.println("state has ball");
        //robot.ui.buzz(900, 10);
        robot.motors.driveToAngle(0, motorsPWM * 0.8, correction);
        if(robot.hasBall()){
          robot.kicker.kick();
        }
        robot.firstDetected = false;
        break;

      case BALL_CLOSE:
        Serial.print(ballIntensity);
        Serial.println(" state ball close");
        //robot.ui.buzz(750, 10);
        robot.motors.driveToAngle(robot.adjustBallAngleClose(ballAngle), motorsPWM * 0.9, correction);
        robot.firstDetected = false;
        break;

      case BALL_FAR:
        Serial.print(ballIntensity);
        Serial.println(" state ball far");
        //robot.ui.buzz(500, 10);
        //robot.motors.driveToAngle(ballAngle, motorsPWM, correction);
        robot.motors.driveToAngle(robot.adjustBallAngleFar(ballAngle), motorsPWM * 0.9, correction);
        robot.firstDetected = false;
        break;

      case IDLE:
        Serial.println("state idle");
        robot.motors.driveToAngle(0, 0, correction);
        robot.firstDetected = false;
        break;
    }
  } else{
    robot.motors.setAllMotorsOutput(0);
  }
  //Serial.println(loopTime);
}