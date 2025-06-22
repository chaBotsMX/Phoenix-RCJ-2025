/*---ATTACK---*/

#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833

int angle = 0;
int intensity = 0;
int distance = 0;

unsigned long timer;

unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  uart.begin(1000000);
}

void loop() {
  //unsigned long loopTime = micros() - lastTime;
  //lastTime = micros();

  ir.update(IR_UPDATE_TIME);
  
  angle = ir.getAngle();
  intensity = ir.getIntensity();
  distance = ir.getDistance();
  
  if(millis() > timer){
    timer = millis() + 4;
    uart.sendInfo(angle, intensity, distance);
    
    Serial.print(angle*2); Serial.print('\t');
    Serial.print(intensity); Serial.print('\t');
    Serial.print(distance); Serial.print('\n');
  }
  //Serial.println(loopTime);
}