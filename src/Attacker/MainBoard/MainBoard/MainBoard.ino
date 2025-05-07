#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"

const int motorsPWM = 70;

IMU imu;
Motors motors;
UART uart;
UI ui;
elapsedMillis camCounter;
PID pid(1.85, 0.1, motorsPWM);
int setpointOffset = 0;
unsigned long long correctionUpdate = 0;
unsigned long printUpdate = 0;
unsigned long cameraUpdate = 0;

int angleIR = 500;
int intensityIR = 0; const int maxIntensityIR = 2200;
int distanceIR = 1000;
int currentOffset = 0;
int angleLine = 500;

int blobX = -1, blobY = -1;

float setpoint = 0;
int correction = 0;
int timerAngle = 0;
bool firstDetected = false;
int firstSector = -1;

int angleCam = -1;
elapsedMillis timerLS;

void setup() {
  Serial.begin(115200);
  uart.beginIR(115200);
  uart.beginLS(115200);
  uart.beginCam(19200);

  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print("1");
  if (!imu.begin()) {
    Serial.println("imu not found");
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
  Serial.print("2");
}

void loop() {
  ui.update();
  uart.receiveInfoIR();
  uart.receiveInfoLS();
  uart.receiveInfoCam();

  angleIR = uart.angleIR; Serial.print(angleIR); Serial.print('\t');
  intensityIR = uart.intensityIR; intensityIR = map(intensityIR, 0, 2000, 0, 100); Serial.println(intensityIR);
  distanceIR = uart.distanceIR;

  angleLine = uart.angleLS;
  Serial.print("linea: "); Serial.println(distanceIR); 
  blobX = uart.blobX;
  blobY = uart.blobY;
  angleCam = getCamAngle(blobY, blobX); Serial.println(angleCam);

  if(ui.rightButtonToggle){

    if(lineDetected()){
      ui.buzz(400, 400);

      if(!firstDetected){
        firstSector = getLineSector(angleLine);
        firstDetected = true;
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
      setpointOffset = setpoint;
    }
    
 

    float yaw = imu.getYaw(); //Serial.println(yaw);
    float error = yaw - setpointOffset;
    if(isGoalVisible() && isBallOnFront() && error > -2 && error < 2 && angleCam != 500 && abs(currentOffset) < 45 ){
      setpointOffset  -= angleCam;
      currentOffset += angleCam;
    } else if (!isBallOnFront()) {
      setpointOffset = setpoint;
      currentOffset = 0;
    }

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

bool isBallOnFront() {
  if (distanceIR < 800 && (angleIR < 25 || angleIR > 335)) {
    camCounter = 0;  // reinicia el contador al detectar algo
    return true;
  }

  if (camCounter < 600) {
    return true;
  }

  return false;
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

int getCamAngle(int x, int y){
  if(x != -1 || y != -1){
    x = x - 125;
    return atan2(y, x) * (180.0 / M_PI) - 90;
  } else{
    return 500;
  }
}

bool isGoalVisible(){
  if(angleCam != 1) return true;
  else return false;
}