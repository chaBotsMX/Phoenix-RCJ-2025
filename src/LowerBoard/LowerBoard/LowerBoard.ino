#include "LineSensor.h"
#include "UART.h"

LineSensor ls;
UART uart;

int angle = 0;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  uart.begin(115200);
  ls.begin();
}

void loop() {
  ls.update();

  angle = ls.getAngle();

  if(millis() > timer){
    timer = millis() + 100;

    uart.sendInfo(angle);
    Serial.println(angle);
  }
}
