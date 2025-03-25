#include "IMU.h"
#include "Motors.h"

IMU imu;
Motors motors;

const int maxRotationPow = 100;

long long nextUpdateDir = 0;
double lastError = 0;

const double kP = -3.16;
const double kD = -0.12;

void setup() {
  Serial.begin(115200);

  if (!imu.begin()) {
    Serial.println("imu not found");
    while (1);
  }
  
}

int i = 0;

void loop() {
  motors.driveToAngle(i, 100, 0);

  if(i > 360){
    i = 0;
  }

  i+=3;

  delay(50);


  /*if (imu.update()) {
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
  }*/
}
