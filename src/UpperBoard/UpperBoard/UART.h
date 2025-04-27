#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){};

    void begin(long baud){
      Serial4.begin(baud);
    };

    void sendInfo(int data1, int data2){
      uint8_t data1High = data1/256;
      uint8_t data1Low = data1%256;
      uint8_t data2High = data2/256;
      uint8_t data2Low = data2%256;
      
      uint8_t checksum = data1High + data1Low + data2High + data2Low;

      Serial4.write(startMarker);
      Serial4.write(data1High);
      Serial4.write(data1Low);
      Serial4.write(data2High);
      Serial4.write(data2Low);
      Serial4.write(checksum);
      Serial4.write(endMarker);
    };
    
  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
};

#endif