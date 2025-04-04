#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"

IMU imu;
Motors motors;
UART uart;
PID pid(3.16, 0.12, 100);

long long nextUpdateDir = 0;

unsigned long printUpdate = 0;

int angleIR = 0;
int intensityIR = 0;

int correction = 0;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  if (!imu.begin()) {
    Serial.println("imu not found");
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
  
}

void loop() {
  uart.receiveInfo();

  angleIR = uart.data1;
  intensityIR = uart.data2;

  if(angleIR != 500){
    motors.driveToAngle(180-angleIR, 100, correction);
  } else{
    motors.driveToAngle(0, 0, correction);
  }

  if (imu.update()) {
    float yaw = imu.getYaw();

    if (millis() > nextUpdateDir) {
      nextUpdateDir = millis() + 10;
      correction = pid.getCorrection(yaw);

      Serial.print("Yaw: "); Serial.println(yaw);
    }
  }
}
