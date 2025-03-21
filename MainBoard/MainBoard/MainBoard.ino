#include "IMU.h"
#include "Motors.h"

IMU imu;
Motors motors;

const int POW = 570;
long long nextUpdateDir = 0;
double lastError = 0;

double kP = 3.16;
double kD = -0.12;

void setup() {
  Serial.begin(115200);

  if (!imu.begin()) {
    Serial.println("imu not found");
    while (1);
  }
}

void loop() {
  if (imu.update()) {
    float yaw = imu.getYaw();

    if (millis() > nextUpdateDir) {
      nextUpdateDir = millis() + 10;
      
      double error = yaw;
      double derivative = (error - lastError) / 0.01;
      int correction = (int) (kP * error) + (kD * (error - lastError) / 0.01);
      correction = constrain(correction, -255, 255);
      lastError = error;

      motors.driveToAngle(45, 100, correction);

      Serial.print("Yaw"); Serial.println(yaw);
      Serial.print("P"); Serial.println(kP * error);
      Serial.print("D"); Serial.println(kD * (error - lastError) / 0.01);

    }
  }
}
