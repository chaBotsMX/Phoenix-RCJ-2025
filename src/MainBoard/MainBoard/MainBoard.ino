#include "IMU.h"
#include "Motors.h"
#include "UART.h"

IMU imu;
Motors motors;
UART uart;

const int maxRotationPow = 100;

long long nextUpdateDir = 0;
double lastError = 0;

const double kP = -3.16;
const double kD = -0.12;

unsigned long printUpdate = 0;

int angleIR = 0;
int intensityIR = 0;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);

  if (!imu.begin()) {
    Serial.println("imu not found");
    while (1);
  }
  
}

void loop() {
  uart.receiveInfo();

  angleIR = uart.data1;
  intensityIR = uart.data2;

  if((millis() - printUpdate) > 100){
    printUpdate = millis();
    
    Serial.print(angleIR); Serial.print('\t');
    Serial.print(intensityIR); Serial.print('\n');
  }
    
/*
  if (imu.update()) {
    float yaw = imu.getYaw();

    if (millis() > nextUpdateDir) {
      nextUpdateDir = millis() + 10;
        
      double error = yaw;
      double derivative = (error - lastError) / 0.01;
      int correction = (int) (kP * error) + (kD * derivative);
      correction = constrain(correction, -maxRotationPow, maxRotationPow);
      lastError = error;

      //motors.setAllMotorsOutput(correction);

      Serial.print("Yaw: "); Serial.println(yaw);
      Serial.print("P: "); Serial.println(kP * error);
      Serial.print("D: "); Serial.println(kD * derivative);
      Serial.print("Correction: "); Serial.println(correction);
    }
  }
  */
}
