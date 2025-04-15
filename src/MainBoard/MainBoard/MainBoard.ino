#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"

IMU imu;
Motors motors;
UART uart;
UI ui;
PID pid(4, 0.112, 100);

unsigned long long correctionUpdate = 0;
unsigned long printUpdate = 0;

int angleIR = 500;
int intensityIR = 0;

int correction = 0;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  if (!imu.begin()) {
    Serial.println("imu not found");
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
  
}

void loop() {
  ui.update();
  uart.receiveInfo();

  angleIR = uart.data1;
  intensityIR = uart.data2;

  if(ui.rightButtonToggle){

    if(angleIR != 500){
      motors.driveToAngle(angleIR, 100, correction);
    } else{
      motors.driveToAngle(0, 0, correction);
    }

  } else{
    motors.setAllMotorsOutput(0);
  }

  if (imu.update()) {
    float yaw = imu.getYaw();

    if (millis() > correctionUpdate) {
      correctionUpdate = millis() + 10;
      correction = pid.getCorrection(yaw);
    }
  }
}
