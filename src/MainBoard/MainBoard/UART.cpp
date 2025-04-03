#include "UART.h"

UART::UART(){
  currentState = WAIT_FOR_START;
}

void UART::begin(long baud){
  Serial7.begin(baud);
}

void UART::receiveInfo(){
  Serial.println(Serial7.read());

  /*if (Serial1.available() > 0) {
    checkData(Serial7.read());
  } else{
    Serial.println("x");
  }*/
}

void UART::checkData(uint8_t incomingByte){
  switch(currentState) {
    case WAIT_FOR_START:
      if(incomingByte == 255) {
        currentState = READ_ANGLE_HIGH;
        calculatedChecksum = 0;
      }
      break;

    case READ_ANGLE_HIGH:
      localData1 = incomingByte * 255;
      calculatedChecksum += incomingByte;
      currentState = READ_ANGLE_LOW;
      break;

    case READ_ANGLE_LOW:
      localData1 += incomingByte;
      calculatedChecksum += incomingByte;
      currentState = READ_INTENSITY_HIGH;
      break;

    case READ_INTENSITY_HIGH:
      localData2 = incomingByte * 255;
      calculatedChecksum += incomingByte;
      currentState = READ_INTENSITY_LOW;
      break;

    case READ_INTENSITY_LOW:
      localData2 += incomingByte;
      calculatedChecksum += incomingByte;
      currentState = READ_CHECKSUM;
      break;

    case READ_CHECKSUM:
      if (calculatedChecksum == incomingByte) currentState = WAIT_FOR_END;
      else currentState = WAIT_FOR_START;
      break;

    case WAIT_FOR_END:
      if (incomingByte == 254) {
        data1 = localData1;
        data2 = localData2;
      } else return;
      currentState = WAIT_FOR_START;
      break;
  }
}