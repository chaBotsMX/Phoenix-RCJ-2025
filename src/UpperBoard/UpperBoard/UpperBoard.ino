#include "IR.h"

IR ir;

void setup() {
  Serial.begin(115200);
}

void loop() {
  ir.update();
  ir.printIR();
}