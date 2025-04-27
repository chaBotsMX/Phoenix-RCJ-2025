#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833
#define IR_DEBUG_TIME 100

int rawAngle = 0;
int angle = 0;
int intensity = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);
}

void loop() {
  ir.update(IR_UPDATE_TIME);
  
  rawAngle = ir.getRawAngle();
  angle = ir.getAngle();
  intensity = ir.getIntensity();
  
  if(millis() > timer){
    timer = millis() + 100;
    uart.sendInfo(angle, intensity);
    /*Serial.print(rawAngle); Serial.print('\t');
    Serial.print(intensity); Serial.print('\t');
    Serial.print(ir.distance); Serial.print('\t');
    Serial.print(angle); Serial.print('\n');*/
    //ir.printIR(rawAngle, intensity, IR_DEBUG_TIME, true);
  }
}