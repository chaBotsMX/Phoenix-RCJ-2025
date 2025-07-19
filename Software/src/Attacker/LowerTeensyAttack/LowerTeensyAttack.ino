/*---ATTACK--*/

#include "LineSensor.h"
#include "UART.h"

LineSensor ls;
UART uart;

int angle = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  ls.begin();
  uart.begin(1000000);
  delay(1000);
}

void loop() {
  ls.update();

  angle = ls.getAngle();

  if(millis() > timer){
    timer = millis() + 6;

    uart.sendInfo(angle);
    ls.printLS();
    Serial.println(angle*2);
  }
}
