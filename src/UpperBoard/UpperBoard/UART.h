#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){};

    void begin(long baud){
      Serial4.begin(baud);
    };

    void sendInfo(int adjustedAngle, int intensity, int rawAngle){
      uint8_t adjustedAngleHigh = adjustedAngle/256;
      uint8_t adjustedAngleLow = adjustedAngle%256;
      uint8_t intensityHigh = intensity/256;
      uint8_t intensityLow = intensity%256;
      uint8_t rawAngleHigh = rawAngle/256;
      uint8_t rawAngleLow = rawAngle%256;
      
      uint8_t checksum = adjustedAngleHigh + adjustedAngleLow + intensityHigh + intensityLow + rawAngleHigh + rawAngleLow;

      Serial4.write(startMarker);
      Serial4.write(adjustedAngleHigh);
      Serial4.write(adjustedAngleLow);
      Serial4.write(intensityHigh);
      Serial4.write(intensityLow);
      Serial4.write(rawAngleHigh);
      Serial4.write(rawAngleLow);
      Serial4.write(checksum);
      Serial4.write(endMarker);
    };
    
  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
};

#endif