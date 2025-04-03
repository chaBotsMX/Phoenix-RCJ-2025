#include "UART.h"

UART::UART(){}

void UART::begin(long baud){
  Serial7.begin(baud);
}

void UART::sendInfo(int data1, int data2){
  /*uint8_t data1High = highByte(data1);
  uint8_t data1Low = lowByte(data1);
  uint8_t data2High = highByte(data2);
  uint8_t data2Low = lowByte(data2);
  uint8_t checksum = data1High + data1Low + data2High + data2Low;

  Serial1.write(startMarker);
  Serial1.write(data1High);
  Serial1.write(data1Low);
  Serial1.write(data2High);
  Serial1.write(data2Low);
  Serial1.write(checksum);
  Serial1.write(endMarker);*/

  Serial7.write(1);
}