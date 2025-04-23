#include "UART.h"

UART::UART(){}

void UART::begin(long baud){
  Serial6.begin(baud);
}

void UART::sendInfo(int data1, int data2){
  uint8_t data1High = data1/256;
  uint8_t data1Low = data1%256;
  uint8_t data2High = data2/256;
  uint8_t data2Low = data2%256;
  
  uint8_t checksum = data1High + data1Low + data2High + data2Low;

  Serial6.write(startMarker);
  Serial6.write(data1High);
  Serial6.write(data1Low);
  Serial6.write(data2High);
  Serial6.write(data2Low);
  Serial6.write(checksum);
  Serial6.write(endMarker);
}