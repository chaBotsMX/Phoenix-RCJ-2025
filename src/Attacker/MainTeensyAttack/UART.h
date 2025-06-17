#ifndef UART_H
#define UART_H

#include <Arduino.h>

#define IRSerial Serial7
#define LineSerial Serial5
#define CameraSerial Serial8
#define DisplaySerial Serial1

class UART {
  public:
    UART(){
      irState = WAIT_FOR_START;
      lineState = WAIT_FOR_START;
    };
    
    void beginIR(long baud){
      IRSerial.begin(baud);
      IRSerial.clear();
    };

    void beginLine(long baud){
      LineSerial.begin(baud);
      LineSerial.clear();
    };

    void beginCamera(long baud){
      CameraSerial.begin(baud);
      CameraSerial.clear();
    };

    void beginDisplay(long baud){
      DisplaySerial.begin(baud);
    };

    void receiveIRData(){
      if (IRSerial.available() > 0) {
        checkIRData(IRSerial.read());
      }
    };

    void receiveLineData(){
      if(LineSerial.available() > 0) {
        checkLineData(LineSerial.read());
      }
    };

    int getIRAngle(){
      return angleIR;
    }

    int getIRIntensity(){
      return intensityIR;
    }

    int getIRDistance(){
      return distanceIR;
    }

    int getLineAngle(){
      return angleLS;
    }

  private:
    int angleIR = 500;
    int intensityIR = 0;
    int distanceIR = 100;

    int angleLS = 500;

    unsigned long lastIRByteTime = 0;
    unsigned long lastLSByteTime = 0;

    enum DataState {
      WAIT_FOR_START,
      READ_ANGLE,
      READ_INTENSITY,
      READ_DISTANCE
    };

    DataState irState = WAIT_FOR_START;

    void checkIRData(uint8_t data){
      switch(irState) {
        case WAIT_FOR_START:
          if(data == 255) irState = READ_ANGLE;
          break;

        case READ_ANGLE:
          angleIR = data * 2;
          irState = READ_INTENSITY;
          break;

        case READ_INTENSITY:
          intensityIR = data;
          irState = READ_DISTANCE;
          break;

        case READ_DISTANCE:
          distanceIR = data;
          irState = WAIT_FOR_START;
          break;
      }
    };

    DataState lineState = WAIT_FOR_START;

    void checkLineData(uint8_t data){
      switch(lineState) {
        case WAIT_FOR_START:
          if(data == 255) lineState = READ_ANGLE;
          break;

        case READ_ANGLE:
          angleLS = data * 2;
          lineState = WAIT_FOR_START;
          break;
      }
    };
};

#endif