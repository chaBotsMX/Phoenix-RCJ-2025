#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833
#define IR_DEBUG_TIME 100

int rawAngle = 0;
int angle = 0;
int intensity = 0;
int distance = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);
}

void loop() {
  ir.update(IR_UPDATE_TIME);
  
  rawAngle = ir.getRawAngle();
  angle = ir.getAngle();
  intensity = ir.getIntensity(); //intensity = map(intensity, 0, 2200, 0, 100);
  distance = ir.getDistance();
  
  if(millis() > timer){
    timer = millis() + 100;
    uart.sendInfo(rawAngle, intensity, distance);
    
    Serial.print(rawAngle); Serial.print('\t');
    Serial.print(map(intensity, 0, 2000, 0, 100)); Serial.print('\t');
    Serial.print(distance); Serial.print('\t');
    Serial.print(angle); Serial.print('\n');
    //ir.printIR(rawAngle, intensity, IR_DEBUG_TIME, true);
  }
}