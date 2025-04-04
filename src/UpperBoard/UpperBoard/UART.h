#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART();
    void begin(long baud);
    void sendInfo(int data1, int data2);
  private:
    const uint8_t startMarker = 255;
    const uint8_t endMarker = 254;
};

#endif