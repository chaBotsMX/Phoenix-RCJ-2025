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

    if(angleIR == 500){
      motors.driveToAngle(0, 0, correction);
    }
    
    else if(intensityIR > 80){
      motors.driveToAngle(adjustAngleIRback2(angleIR), motorsPWM * 0.7, correction);
    }

    else if(distanceIR < 850 || (angleIR < 25 && angleIR > 335)){
      motors.driveToAngle(0, motorsPWM, correction);
    }

    else{
      //motors.driveToAngle(adjustAngleIRfront(angleIR), motorsPWM, correction);
      motors.driveToAngle(angleIR, motorsPWM * 0.9, correction);
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
      //Serial.print(yaw); Serial.print('\n');
    }
  }
}

int adjustAngleIRback(int angle){
  if(angle != 500){
    if(angle > 120 && angle < 180){
      return angle + 90;
    } else if(angle < 240 && angle >= 180){
      return angle - 90;
    } else{
      return angle;
    }
  } else{
    return 500;
  }
}

int adjustAngleIRback2(int angle){
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

/*
#include "Motors.h"

Motors motors;

void setup(){}

void loop(){
  motors.setAllMotorsOutput(50);
}*/