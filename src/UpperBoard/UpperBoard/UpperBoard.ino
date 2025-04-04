#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833
#define IR_DEBUG_TIME 100

int angle = 0;
int intensity = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);
}

void loop() {
  ir.update(IR_UPDATE_TIME);
  
  angle = ir.getAngle();
  intensity = ir.getIntensity();
  
  if(millis() > timer){
    timer = millis() + 100;
    uart.sendInfo(angle, intensity);
    Serial.print(angle); Serial.print('\t');
    Serial.print(intensity); Serial.print('\n');
    //ir.printIR(angle, intensity, IR_DEBUG_TIME);
  }
}