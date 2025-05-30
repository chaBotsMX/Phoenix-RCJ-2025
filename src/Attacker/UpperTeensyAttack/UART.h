#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){};

    void begin(long baud){
      Serial4.begin(baud);
    };

    void sendInfo(int angle, int intensity, int distance){
      uint8_t angleHigh = angle/256;
      uint8_t angleLow = angle%256;
      uint8_t intensityHigh = intensity/256;
      uint8_t intensityLow = intensity%256;
      uint8_t distanceHigh = distance/256;
      uint8_t distanceLow = distance%256;
      
      uint8_t checksum = angleHigh + angleLow + intensityHigh + intensityLow + distanceHigh + distanceLow;

      Serial4.write(startMarker);
      Serial4.write(angleHigh);
      Serial4.write(angleLow);
      Serial4.write(intensityHigh);
      Serial4.write(intensityLow);
      Serial4.write(distanceHigh);
      Serial4.write(distanceLow);
      Serial4.write(checksum);
      Serial4.write(endMarker);
    };
    
  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
};

#endif