#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833
#define IR_DEBUG_TIME 100

int angle = 0;
int intensity = 0;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);
}

void loop() {
  ir.update(IR_UPDATE_TIME);
  
  angle = ir.getAngle();
  intensity = ir.getIntensity();
  
  uart.sendInfo(angle, intensity);

  ir.printIR(angle, intensity, IR_DEBUG_TIME, true);
}