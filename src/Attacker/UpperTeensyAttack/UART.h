#ifndef UART_H
#define UART_H

#include <Arduino.h>

#define IRSerial Serial4

class UART {
  public:
    UART(){};

    void begin(long baud){
      IRSerial.begin(baud);
    };

    void sendInfo(uint8_t angle, uint8_t intensity, uint8_t distance){
      IRSerial.write(startMarker);
      IRSerial.write(angle);
      IRSerial.write(intensity);
      IRSerial.write(distance);
    };
    
  private:
    const uint8_t startMarker = 255;
};

#endif