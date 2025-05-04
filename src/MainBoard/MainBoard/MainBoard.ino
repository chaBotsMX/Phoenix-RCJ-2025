#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"

const int motorsPWM = 80;

IMU imu;
Motors motors;
UART uart;
UI ui;
PID pid(1.85, 0.1, motorsPWM);

unsigned long long correctionUpdate = 0;
unsigned long printUpdate = 0;

int angleIR = 500;
int intensityIR = 0; const int maxIntensityIR = 2200;
int distanceIR = 1000;

int angleLine = 500;

float setpoint = 0;
int correction = 0;

bool firstDetected = false;
int firstSector = -1;

void setup() {
  Serial.begin(115200);
  uart.beginIR(115200);
  uart.beginLS(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  if (!imu.begin()) {
    Serial.println("imu not found");
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }  
}

void loop() {
  ui.update();
  uart.receiveInfoIR();
  uart.receiveInfoLS();

  angleIR = uart.angleIR;
  intensityIR = uart.intensityIR; intensityIR = map(intensityIR, 0, 2000, 0, 100);
  distanceIR = uart.distanceIR;

  angleLine = uart.angleLS;

  if(ui.rightButtonToggle){

    if(lineDetected()){
      ui.buzz(400, 400);

      if(!firstDetected){
        firstSector = getLineSector(angleLine);
      }
      int sector = getLineSector(angleLine);
      int avoidAngle = adjustAngleLine(line_switch(sector, firstSector));

      motors.driveToAngle(avoidAngle, motorsPWM, correction);
    }

    else if(!ballDetected()){
      motors.driveToAngle(0, 0, correction);
      firstDetected = false;
    }
    
    else if(intensityIR > 75){
      motors.driveToAngle(adjustAngleIR(angleIR), motorsPWM * 0.8, correction);
      firstDetected = false;
    }

    else if(isBallOnFront()){
      motors.driveToAngle(0, motorsPWM, correction);
      firstDetected = false;
    }

    else{
      motors.driveToAngle(angleIR, motorsPWM, correction);
      firstDetected = false;
    }

  } else{
    motors.setAllMotorsOutput(0);
  }
  
  if (imu.update()) {
    if(ui.leftButtonState){
      setpoint = imu.getYaw();
    }

    float yaw = imu.getYaw();
    float error = yaw - setpoint;

    if (millis() > correctionUpdate) {
      correctionUpdate = millis() + 10;
      correction = pid.getCorrection(error);
    }
  }
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

bool isBallOnFront(){
  if(distanceIR < 850 || (angleIR < 25 && angleIR > 335)) return true;
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