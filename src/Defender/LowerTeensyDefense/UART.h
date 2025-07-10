#ifndef UART_H
#define UART_H

#include <Arduino.h>

#define LineSerial Serial2

class UART {
  public:
    UART(){};

    void begin(long baud){
      LineSerial.begin(baud);
    };

    void sendInfo(uint8_t angle, uint8_t depth, uint8_t side){
      LineSerial.write(startMarker);
      LineSerial.write(angle);
      LineSerial.write(depth);
      LineSerial.write(side);
    };

  private:
    const uint8_t startMarker = 255;
};

#endif