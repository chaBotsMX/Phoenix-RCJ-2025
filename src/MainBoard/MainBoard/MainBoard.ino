#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"

IMU imu;
Motors motors;
UART uart;
UI ui;
PID pid(1.85, 0.1, 60);

#define motorsPWM 50

unsigned long long correctionUpdate = 0;
unsigned long printUpdate = 0;

int angleIR = 500;
int intensityIR = 0;

int angleLine = 500;

int correction = 0;

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
  angleLine = uart.angleLS;

  if(ui.rightButtonToggle){

    if(angleIR == 500){
      motors.driveToAngle(0, 0, correction);
    }
    
    /*else if(intensityIR > 90){
      motors.driveToAngle(adjustAngleIR(angleIR), motorsPWM, correction);
    }*/
    
    else{
      motors.driveToAngle(adjustAngleIR(angleIR), motorsPWM, correction);
    }

  } else{
    motors.setAllMotorsOutput(0);
  }

  if (imu.update()) {
    float yaw = imu.getYaw();

    if (millis() > correctionUpdate) {
      correctionUpdate = millis() + 10;
      correction = pid.getCorrection(yaw);
      //Serial.print(yaw); Serial.print('\n');
    }
  }
}

int adjustAngleIR(int angle){
  if(angle != 500){
    if(angle > 90 && angle < 180){
      return angle + 90;
    } else if(angle < 270 && angle >= 180){
      return angle - 90;
    } else{
      return angle;
    }
  } else{
    return 500;
  }
}

/*
#include "Motors.h"

Motors motors;

void setup(){}

void loop(){
  motors.setAllMotorsOutput(50);
}*/