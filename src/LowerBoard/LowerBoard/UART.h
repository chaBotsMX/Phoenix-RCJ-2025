#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
  
  public:
    UART(){};

    void begin(long baud){
      Serial5.begin(baud);
    };

    void sendInfo(int data){
      uint8_t dataHigh = data/256;
      uint8_t dataLow = data%256;
      
      uint8_t checksum = dataHigh + dataLow;

      Serial5.write(startMarker);
      Serial5.write(dataHigh);
      Serial5.write(dataLow);
      Serial5.write(checksum);
      Serial5.write(endMarker);
    };

};

#endif