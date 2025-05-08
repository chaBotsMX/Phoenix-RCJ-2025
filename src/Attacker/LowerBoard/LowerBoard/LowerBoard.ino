/*---ATTACK--*/

#include "LineSensor.h"
#include "UART.h"

LineSensor ls;
UART uart;

int angle = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while(millis() - start < 1000){}
  ls.begin();
  uart.begin(115200);
}

void loop() {
  ls.update();

  angle = ls.getAngle();

  if(millis() > timer){
    timer = millis() + 50;

    uart.sendInfo(angle);
    ls.printLS();
    Serial.println(angle);
  }
}
