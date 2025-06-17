/*---ATTACK---*/

/*#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"
#include "Kicker.h"

const int motorsPWM = 40;

IMU imu;
Motors motors;
UART uart;
UI ui;
Kicker kicker;
PID pid(1.85, 0.1, motorsPWM);

unsigned long long updateTimer = 0;

int angleIR = 500;
int intensityIR = 0;
int distanceIR = 1000;
int angleLine = 500;

float setpoint = 0;
int correction = 0;
bool firstDetected = false;
int firstSector = -1;

void setup() {
  Serial.begin(115200);
  uart.beginIR(1000000);
  uart.beginLine(115200);

  delay(1000);

  if (!imu.begin()) {
    Serial.println("imu not found");
    ui.buzz(800, 400);
  }
}

void loop() {
  uart.receiveIRData();
  uart.receiveLineData();

  angleIR = uart.getIRAngle(); Serial.print(angleIR); Serial.print('\t');
  intensityIR = uart.getIRIntensity(); Serial.print(intensityIR); Serial.print('\t');
  distanceIR = uart.getIRDistance(); Serial.println(distanceIR);

  angleLine = uart.getLineAngle(); //Serial.println(angleLine);

  if(millis() - updateTimer >= 10){
    updateTimer = millis();
    ui.update();
    imu.update();
    updatePID();
  }

  if(ui.rightButtonToggle){
    if(lineDetected()){
      ui.buzz(400, 400);

      if(!firstDetected){
        firstSector = getLineSector(angleLine);
        firstDetected = true;
      }
      int sector = getLineSector(angleLine);
      int avoidAngle = adjustAngleLine(line_switch(sector, firstSector));

      motors.driveToAngle(avoidAngle, motorsPWM * 1.2, correction);
    }

    else if(ballDetected()){
      motors.driveToAngle(angleIR, motorsPWM, correction);
      firstDetected = false;
    }
    
    else if(robotHasBall()){
      ui.buzz(1000, 500);
      kicker.kick();
      motors.driveToAngle(0, motorsPWM * 0.8, correction);
      firstDetected = false;
    }

    else if(isBallOnFront()){
      motors.driveToAngle(0, motorsPWM * 0.9, correction);
      firstDetected = false;
    }

    else if(intensityIR > 75){
      motors.driveToAngle(adjustAngleIR(angleIR), motorsPWM * 0.9, correction);
      firstDetected = false;
    }

    else{
      motors.driveToAngle(0, 0, correction);
      firstDetected = false;
    }

  } else{
    motors.setAllMotorsOutput(0);
  }
}

void updatePID(){
  if(ui.leftButtonState){
    setpoint = imu.getYaw();
  } 

  float yaw = imu.getYaw(); //Serial.println(yaw);
  float error = yaw - setpoint;
  correction = pid.getCorrection(error);
}

int adjustAngleIR(int angle){
  if(angle != 500){
    if(angle > 25 && angle < 180){
      return angle + 90;
    } else if(angle < 335 && angle >= 180){
      return angle - 90;
    } else{
      return angle;
    }
  } else{
    return 500;
  }
}

int adjustAngleLine(int angle){
  if(angle >= 0 && angle < 180){
    return angle + 180;
  } else if(angle <= 360 && angle >= 180){
    return angle - 180;
  } else{
    return angle;
  }
}

bool isBallOnFront() {
  if (intensityIR > 80 && (angleIR < 35 || angleIR > 340) && distanceIR < 800) return true;
  else return false;
}
bool lineDetected(){
  if(angleLine == 500) return false;
  else return true;
}

bool ballDetected(){
  if(angleIR == 500) return false;
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

bool robotHasBall(){
  //if(intensityIR > 80 && distanceIR < 800) return true; //&& (angleIR < 25 || angleIR > 335)) return true;
  //else return false;
  static unsigned long ballSeenSince = 0;
  static bool tracking = false;

  bool currentBallState = ((intensityIR > 70 && distanceIR < 600) || (angleIR < 35 || angleIR > 320));

  if (currentBallState) {
    if (!tracking) {
      ballSeenSince = millis();
      tracking = true;
    }
    if (millis() - ballSeenSince >= 200) {
      return true;
    }
  } else {
    tracking = false;
    ballSeenSince = 0;
  }

  return false;
}*/

#include "Robot.h"

const int motorsPWM = 40;

Robot robot(motorsPWM);

auto& ballAngle = robot.ballAngle;
auto& ballIntensity = robot.ballIntensity;
auto& ballDistance = robot.ballDistance;
auto& lineAngle = robot.lineAngle;
auto& correction = robot.correction;

void setup() {
  Serial.begin(115200);
  robot.uart.beginIR(1000000);
  robot.uart.beginLine(115200);

  delay(1000);

  if (!robot.imu.begin()) {
    Serial.println("imu not found");
    robot.ui.buzz(800, 400);
  }
}

void loop() {
  robot.uart.receiveIRData();
  robot.uart.receiveLineData();

  ballAngle = robot.uart.getIRAngle(); Serial.print(ballAngle); Serial.print('\t');
  ballIntensity = robot.uart.getIRIntensity(); Serial.print(ballIntensity); Serial.print('\t');
  ballDistance = robot.uart.getIRDistance(); Serial.println(ballDistance);

  lineAngle = robot.uart.getLineAngle(); //Serial.println(angleLine);

  if(millis() - robot.updateTimer >= 10){
    robot.updateTimer = millis();
    robot.ui.update();
    if(robot.imu.update()) robot.updatePID();
  }

  if(robot.ui.rightButtonToggle){
    if(robot.lineDetected()){
      Serial.println("1");
      robot.ui.buzz(400, 400);

      if(!robot.firstDetected){
        robot.firstSector = robot.getLineSector(lineAngle);
        robot.firstDetected = true;
      }
      int sector = robot.getLineSector(lineAngle);
      int avoidAngle = robot.adjustLineAngle(robot.line_switch(sector, robot.firstSector));

      robot.motors.driveToAngle(avoidAngle, motorsPWM * 1.2, correction);
    }
    
    else if(robot.hasBall()){
      Serial.println("3");
      robot.ui.buzz(1000, 500);
      robot.kicker.kick();
      robot.motors.driveToAngle(0, motorsPWM * 0.8, correction);
      robot.firstDetected = false;
    }

    else if(robot.isBallOnFront()){
      Serial.println("4");
      robot.motors.driveToAngle(0, motorsPWM * 0.9, correction);
      robot.firstDetected = false;
    }

    else if(ballIntensity > 75){
      Serial.println("5");
      robot.motors.driveToAngle(robot.adjustBallAngle(ballAngle), motorsPWM * 0.9, correction);
      robot.firstDetected = false;
    }

    else if(robot.ballDetected()){
      Serial.println("2");
      robot.motors.driveToAngle(ballAngle, motorsPWM, correction);
      robot.firstDetected = false;
    }

    else{
      robot.motors.driveToAngle(0, 0, correction);
      robot.firstDetected = false;
    }

  } else{
    robot.motors.setAllMotorsOutput(0);
  }
}