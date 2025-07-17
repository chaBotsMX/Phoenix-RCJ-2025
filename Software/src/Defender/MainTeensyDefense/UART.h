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
      while (IRSerial.available()) {
        checkIRData(IRSerial.read());
      }
    };

    void receiveLineData(){
      while (LineSerial.available()) {
        checkLineData(LineSerial.read());
      }
    };

    void receiveCameraData(){
      if(CameraSerial.available()){
        checkCameraData(CameraSerial.read());
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

    int getLineDepth(){
      return depthLS;
    }

    int getLineSide(){
      return sideLS;
    }

    int getBlobX(){
      return blobX;
    }

    int getBlobArea(){
      return blobArea;
    }

    int getBluetoothSignal(){
      return bluetoothSignal;
    }

  private:
    int angleIR = 500;
    int intensityIR = 0;
    int distanceIR = 100;

    int angleLS = 500;
    int depthLS = -1;
    int sideLS = -1;

    int blobX = 250, blobArea = 250;

    int bluetoothSignal = -1;

    unsigned long lastIRByteTime = 0;
    unsigned long lastLSByteTime = 0;

    enum DataState {
      WAIT_FOR_START,
      DATA_1,
      DATA_2,
      DATA_3,
      DATA_4
    };

    DataState irState = WAIT_FOR_START;

    void checkIRData(uint8_t data){
      switch(irState) {
        case WAIT_FOR_START:
          if(data == 255) irState = DATA_1;
          break;

        case DATA_1:
          angleIR = data * 2;
          irState = DATA_2;
          break;

        case DATA_2:
          intensityIR = data;
          irState = DATA_3;
          break;

        case DATA_3:
          distanceIR = data;
          irState = DATA_4;
          break;

        case DATA_4:
          bluetoothSignal = data;
          irState= WAIT_FOR_START;
          break;
      }
    };

    DataState lineState = WAIT_FOR_START;

    void checkLineData(uint8_t data){
      switch(lineState) {
        case WAIT_FOR_START:
          if(data == 255) lineState = DATA_1;
          break;

        case DATA_1:
          angleLS = data * 2;
          lineState = DATA_2;
          break;

        case DATA_2:
          depthLS = data;
          lineState = DATA_3;
          break;

        case DATA_3:
          sideLS = data;
          lineState = WAIT_FOR_START;
          break;
      }
    };

    DataState cameraState = WAIT_FOR_START;

    void checkCameraData(uint8_t data){
      switch(cameraState){
        case WAIT_FOR_START:
          if(data == 255) cameraState = DATA_1;
          break;

        case DATA_1:
          blobX = data * 2;
          cameraState = DATA_2;
          break;

        case DATA_2:
          blobArea = data;
          cameraState = WAIT_FOR_START;
          break;
      }
    }
};

#endif