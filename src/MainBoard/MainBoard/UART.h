#ifndef UART_H
#define UART_H

#include <Arduino.h>

class UART {
  public:
    UART(){
      currentStateIR = WAIT_FOR_START_IR;
      currentStateLS = WAIT_FOR_START_LS;
    };
    
    void beginIR(long baud){
      Serial7.begin(baud);
    };

    void beginLS(long baud){
      Serial5.begin(baud);
    };

    void beginCam(long baud){
      Serial8.begin(baud);
    };

    void beginDisplay(long baud){
      Serial1.begin(baud);
    };

    void receiveInfoIR(){
      if (Serial7.available() > 0) {
        while(Serial7.available()){
          checkDataIR(Serial7.read());
        }
      }
    };

    void receiveInfoLS(){
      if(Serial5.available() > 0) {
        checkDataLS(Serial5.read());
      }
    }

    int angleIR = 500;
    int intensityIR = 0;

    int angleLS = 0;

  private:
    uint8_t checksumIR = 0;

    enum StateIR {
      WAIT_FOR_START_IR,
      READ_ANGLE_HIGH_IR,
      READ_ANGLE_LOW_IR,
      READ_INTENSITY_HIGH_IR,
      READ_INTENSITY_LOW_IR,
      READ_CHECKSUM_IR,
      WAIT_FOR_END_IR
    };

    StateIR currentStateIR;
    int localAngleIR = 500;
    int localIntensityIR = 0;

    void checkDataIR(uint8_t incomingByte){
      switch(currentStateIR) {
        case WAIT_FOR_START_IR:
          if(incomingByte == 255) {
            currentStateIR = READ_ANGLE_HIGH_IR;
            checksumIR = 0;
          }
          break;

        case READ_ANGLE_HIGH_IR:
          localAngleIR = incomingByte * 256;
          checksumIR += incomingByte;
          currentStateIR = READ_ANGLE_LOW_IR;
          break;

        case READ_ANGLE_LOW_IR:
          localAngleIR += incomingByte;
          checksumIR += incomingByte;
          currentStateIR = READ_INTENSITY_HIGH_IR;
          break;

        case READ_INTENSITY_HIGH_IR:
          localIntensityIR = incomingByte * 256;
          checksumIR += incomingByte;
          currentStateIR = READ_INTENSITY_LOW_IR;
          break;

        case READ_INTENSITY_LOW_IR:
          localIntensityIR += incomingByte;
          checksumIR += incomingByte;
          currentStateIR = READ_CHECKSUM_IR;
          break;

        case READ_CHECKSUM_IR:
          if (checksumIR == incomingByte) currentStateIR = WAIT_FOR_END_IR;
          else currentStateIR = WAIT_FOR_START_IR;
          break;

        case WAIT_FOR_END_IR:
          if (incomingByte == 254) {
            angleIR = localAngleIR;
            intensityIR = localIntensityIR;
            Serial7.clear();
          } else return;
          currentStateIR = WAIT_FOR_START_IR;
          break;
      }
    };

    uint8_t checksumLS = 0;

    enum StateLS {
      WAIT_FOR_START_LS,
      READ_ANGLE_HIGH_LS,
      READ_ANGLE_LOW_LS,
      READ_CHECKSUM_LS,
      WAIT_FOR_END_LS
    };

    StateLS currentStateLS;
    int localAngleLS = 0;

    void checkDataLS(uint8_t incomingByte){
      switch(currentStateLS) {
        case WAIT_FOR_START_LS:
          if(incomingByte == 255) {
            currentStateLS = READ_ANGLE_HIGH_LS;
            checksumLS = 0;
          }
          break;

        case READ_ANGLE_HIGH_LS:
          localAngleLS = incomingByte * 256;
          checksumLS += incomingByte;
          currentStateLS = READ_ANGLE_LOW_LS;
          break;

        case READ_ANGLE_LOW_LS:
          localAngleLS += incomingByte;
          checksumLS += incomingByte;
          currentStateLS = READ_CHECKSUM_LS;
          break;

        case READ_CHECKSUM_LS:
          if (checksumLS == incomingByte) currentStateLS = WAIT_FOR_END_LS;
          else currentStateLS = WAIT_FOR_START_LS;
          break;

        case WAIT_FOR_END_LS:
          if (incomingByte == 254) {
            angleLS = localAngleLS;
          } else return;
          currentStateLS = WAIT_FOR_START_LS;
          break;
      }
    };
};

#endif