#ifndef IMU_H
#define IMU_H

#include <Adafruit_BNO08x.h>
#include <Arduino.h>

class IMU {
  public:
    IMU();
    bool begin();
    bool update();
    float getYaw();
  private:
    Adafruit_BNO08x bno08x;
    sh2_SensorValue_t sensorValue;
    float yaw;
};

#endif