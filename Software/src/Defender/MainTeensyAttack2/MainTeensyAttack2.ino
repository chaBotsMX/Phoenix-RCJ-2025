/*---ATTACK---*/

#include "Robot.h"

const int motorsPWM = 230;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& correction = robot.correction;
auto& blobX = robot.blobX;
auto& blobY = robot.blobY;

unsigned long lastTime;

enum RobotState{
  HAS_BALL,
  RETURN_FIELD,
  AVOID_LINE,
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
  else if(blobY == 1){
    return RETURN_FIELD;
  }
  else if (robot.ballDetected()) {
    if(ballIntensity < 150) return BALL_FAR;
    return BALL_CLOSE;
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

  ballAngle = robot.uart.getIRAngle();
  ballIntensity = robot.uart.getIRIntensity();
  ballDistance = robot.uart.getIRDistance();

  lineAngle = robot.uart.getLineAngle();

  blobX = robot.uart.getBlobX();
  blobY = robot.uart.getBlobY();

  robot.bluetoothSignal = robot.uart.getBluetoothSignal();

  robot.kicker.update();

  //robot.offset = 0;

  if(millis() - robot.updateTimer >= 10){
    robot.updateTimer = millis();
    Serial.print("blob x "); Serial.println(blobX);
    Serial.print("blob area "); Serial.println(blobY);
    //Serial.print("ball angle "); Serial.println(ballAngle);
    //Serial.print("intensity "); Serial.println(ballIntensity);
    Serial.print("lineangle "); Serial.println(lineAngle);
    Serial.print("distance "); Serial.println(ballDistance);
    //Serial.print("correction "); Serial.println(correction);
    //Serial.print("yaw "); Serial.println(robot.imu.getYaw());
    Serial.print("state "); Serial.println(currentState);
    //Serial.println(robot.ballDetected());

    robot.ui.update();
    if(robot.imu.update()) robot.updatePID();
  }

  //if(robot.ui.rightButtonToggle){
  if(robot.ui.rightButtonToggle || robot.bluetoothSignal == 1){
    robot.motors.driveToAngle(ballAngle, motorsPWM, correction);
    currentState = determineState();

    switch(currentState){
      case RETURN_FIELD:
        if(robot.goalDetected()){
          if(blobX <= 120 && blobX > 0){//goal left
            robot.motors.driveToAngle(135, motorsPWM * 1.2, correction);
          } else if(blobX >= 200 && blobX < 500){ //goal right
            robot.motors.driveToAngle(225, motorsPWM * 1.2, correction);
          } else{
            robot.motors.driveToAngle(180, motorsPWM * 1.1, correction);
          } 
        } else{
          robot.motors.driveToAngle(0, 0, correction);
        }
        //robot.motors.driveToAngle(0, 0, correction);
        break;

      case AVOID_LINE: {
        robot.ui.buzz(400, 400);

        if(!robot.firstDetected){
          robot.firstSector = robot.getLineSector(lineAngle);
          robot.firstDetected = true;
        }
        int sector = robot.getLineSector(lineAngle);
        int avoidAngle = robot.adjustLineAngle(robot.line_switch(sector, robot.firstSector));

        int sumAngle = robot.getCombinedAvoidanceAngle(ballAngle, avoidAngle);

        robot.motors.driveToAngle(avoidAngle, motorsPWM * 1.2, correction);

        //robot.motors.driveToAngle(sumAngle, motorsPWM*1.2, correction);
        break;
      }
      
      case HAS_BALL:
        if(robot.hasBall()){
          robot.kicker.kick();
        }
        robot.motors.driveToAngle(0, motorsPWM * 0.9, correction);
        robot.firstDetected = false;
        break;

      case BALL_CLOSE:
        robot.motors.driveToAngle(robot.adjustBallAngleClose(ballAngle), motorsPWM, correction);
        robot.firstDetected = false;
        break;

      case BALL_FAR:
        robot.motors.driveToAngle(robot.adjustBallAngleFar(ballAngle), motorsPWM * 0.9, correction);
        robot.firstDetected = false;
        break;

      case IDLE:
        robot.motors.driveToAngle(0, 0, correction);
        robot.firstDetected = false;
        break;
    }
  } else{
    robot.motors.setAllMotorsOutput(0);
  }
  //Serial.println(loopTime);
}