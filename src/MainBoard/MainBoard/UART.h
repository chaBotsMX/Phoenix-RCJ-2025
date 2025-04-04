#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART();
    void begin(long baud);
    void receiveInfo();
    int data1 = 0;
    int data2 = 0;
  private:
    uint8_t calculatedChecksum = 0;
    enum State {
      WAIT_FOR_START,
      READ_ANGLE_HIGH,
      READ_ANGLE_LOW,
      READ_INTENSITY_HIGH,
      READ_INTENSITY_LOW,
      READ_CHECKSUM,
      WAIT_FOR_END
    };
    State currentState;
    int localData1 = 0;
    int localData2 = 0;
    void checkData(uint8_t incomingByte);
};

#endif