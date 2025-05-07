#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){};

    void begin(long baud){
      Serial2.begin(baud);
    };

    void sendInfo(int angle ,int err){
      if(angle != 500) angle+=180;
      uint8_t angleHigh = angle/256;
      uint8_t angleLow = angle%256;
      err < 0 ? err = 200  - abs(err)  : err = err; 
      uint8_t checksum = angleHigh + angleLow;

      Serial2.write(startMarker);
      Serial2.write(angleHigh);
      Serial2.write(angleLow);
      Serial2.write(err);
      Serial2.write(checksum);
      Serial2.write(endMarker);
    };

  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
};

#endif