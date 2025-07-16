/*---DEFENSE---*/

#include "Robot.h"

const int motorsPWM = 200;
const int basePWM = 55;

int lastLineSide = -1;
int lastDepth = -1;

bool robotInStraight = false;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& lineDepth = robot.lineDepth;
auto& lineSide = robot.lineSide;
auto& blobX = robot.blobX;
auto& blobArea = robot.blobArea;
auto& yawCorrection = robot.yawCorrection;
auto& lineCorrection = robot.lineCorrection;

unsigned long lastTime;

enum RobotState{
  CATCH_BALL,
  STAY_IN_LINE,
  RETURN_GOAL,
  ATTACK,
  IDLE
};

RobotState currentState = IDLE;

RobotState determineState() {
  if(!robot.lineDetected()){
    return RETURN_GOAL;
  } else if (robot.lineDetected() && robot.ballDetected()) {
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
  robot.uart.beginCamera(19200);

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
  robot.uart.receiveCameraData();

  ballAngle = robot.uart.getIRAngle(); //Serial.print(ballAngle); Serial.print('\t');
  ballIntensity = robot.uart.getIRIntensity(); //Serial.print(ballIntensity); Serial.print('\t');
  ballDistance = robot.uart.getIRDistance(); //Serial.println(ballDistance);

  lineAngle = robot.uart.getLineAngle(); //Serial.println(lineAngle);
  lineDepth = robot.uart.getLineDepth(); //Serial.print("depth: "); Serial.println(lineDepth);
  lineSide = robot.uart.getLineSide(); //Serial.print("side: "); Serial.println(lineSide);

  blobX = robot.uart.getBlobX();
  blobArea = robot.uart.getBlobArea();

  robot.kicker.update();

  if(millis() - robot.updateTimer >= 10){
    robot.updateTimer = millis();
    robot.ui.update();
    Serial.print("x "); Serial.println(blobX);
    Serial.print("area "); Serial.println(blobArea);  
    if(robot.imu.update()) robot.updateYawPID();
    if(robot.lineDetected()) robot.updateLinePID();
  }

  if(robot.ui.rightButtonToggle){
    currentState = determineState();

    switch(currentState){
      case CATCH_BALL: {
        if(lineSide == 0){ //if line is straight
          robotInStraight = true;

          if(lineDepth > 4){
            robot.motors.driveToAngle(0, basePWM + abs(lineCorrection), yawCorrection); //if line too up, go up
          } else if(lineDepth < 4){
            robot.motors.driveToAngle(180, basePWM + abs(lineCorrection), yawCorrection); //if line too down, go down
          }
          
          else{
            if(ballDistance < 80){ //if ball is in front
              robot.motors.driveToAngle(0, 0, yawCorrection);
            } else if(ballAngle <= 180 && ballAngle > 20){ //if ball is at right
              robot.motors.driveToAngle(90, motorsPWM * 1.1, yawCorrection);
            } else if(ballAngle > 180 && ballAngle < 340){
              robot.motors.driveToAngle(270, motorsPWM * 1.1, yawCorrection);
            } else{
              robot.motors.driveToAngle(0, 0, yawCorrection);
            }
          }
        }
        
        else{
          robotInStraight = false;
          if(robot.isLineSideStable(lineSide, 400)){ //if robot is at corner
            if((ballAngle >= 20 && ballAngle < 180) && (lineSide == 1 || lastLineSide == 1)){ //if ball is at right and corner is left
              robot.motors.driveToAngle(90, motorsPWM * 1.2, yawCorrection);
            } else if((ballAngle <= 340 && ballAngle > 180) && (lineSide == 3 || lastLineSide == 3)){ //if ball is at left and corner is right
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
          if(lineDepth > 4){
            robot.motors.driveToAngle(0, basePWM + abs(lineCorrection), yawCorrection);
          } else if(lineDepth < 4){
            robot.motors.driveToAngle(180, basePWM + abs(lineCorrection), yawCorrection);
          } else{
            robot.motors.driveToAngle(0, 0, yawCorrection);
          }
        } else{
          if(robot.isLineSideStable(lineSide, 500)){
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

      case RETURN_GOAL: //camera
        if(robot.goalDetected()){
          if(blobArea == 1){ //if too inside defense area
            robot.motors.driveToAngle(0, motorsPWM, yawCorrection);
          }
          else if(blobArea == 2){ //if too far
            if(blobX < 100){ // if left
              robot.motors.driveToAngle(158, motorsPWM, yawCorrection);
            } else if(blobX > 220 && blobX < 500){
              robot.motors.driveToAngle(202, motorsPWM, yawCorrection);
            } else{
              robot.motors.driveToAngle(180, motorsPWM, yawCorrection);
            }
          } else{
            if(blobX < 100){ // if left
              robot.motors.driveToAngle(80, motorsPWM, yawCorrection);
            } else if(blobX > 220 && blobX < 500){
              robot.motors.driveToAngle(260, motorsPWM, yawCorrection);
            } else{
              robot.motors.driveToAngle(180, motorsPWM, yawCorrection);
            }
          }
        }  else{
          robot.motors.driveToAngle(0, 0, yawCorrection);
        }
        break;
      case IDLE:
        robot.motors.driveToAngle(0, 0, yawCorrection);
        break;
    }
    if(lastDepth != 15) lastDepth = lineDepth;
    else lastDepth = 15;
    lastLineSide = lineSide;
  } else{
    robot.motors.setAllMotorsOutput(0);
  }
  //Serial.println(loopTime);
}