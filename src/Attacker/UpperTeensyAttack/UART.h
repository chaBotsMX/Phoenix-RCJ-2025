#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){};

    void begin(long baud){
      Serial4.begin(baud);
    };

    void sendInfo(uint8_t angle, uint8_t intensity, uint8_t distance){
      Serial4.write(startMarker);
      Serial4.write(angle);
      Serial4.write(intensity);
      Serial4.write(distance);
    };
    
  private:
    const uint8_t startMarker = 255;
};

#endif