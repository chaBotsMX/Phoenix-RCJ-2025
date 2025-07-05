/*---DEFENSE--*/

#include "LineSensor.h"
#include "UART.h"

LineSensor ls;
UART uart;

int angle = 0;
int depth = -1;

unsigned long timer;

void setup() {
  Serial.begin(115200);
  unsigned long start = millis();
  while(millis() - start < 1000){}
  ls.begin();
  uart.begin(1000000);
  delay(1000);
}

void loop() {
  ls.update();

  angle = ls.getAngle();
  depth = ls.getDepth();

  if(millis() > timer){
    timer = millis() + 2;

    uart.sendInfo(angle, depth);
    ls.printLS();
    Serial.print("Depth: "); Serial.println(depth);
    Serial.print("Angle: "); Serial.println(angle*2);
  }
}
