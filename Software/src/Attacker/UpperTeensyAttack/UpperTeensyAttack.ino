/*---ATTACK---*/

#include "IR.h"
#include "UART.h"

IR ir;
UART uart;

#define IR_UPDATE_TIME 833
//#define bluetooth 7
int angle = 0;
int intensity = 0;
int distance = 0;

//bool bluetoothSignal = false;

unsigned long timer;

unsigned long lastTime;

void setup() {
  Serial.begin(115200);
  uart.begin(1000000);
  //pinMode(bluetooth, INPUT);
  delay(1000);
}

void loop() {
  //unsigned long loopTime = micros() - lastTime;
  //lastTime = micros();

  ir.update(IR_UPDATE_TIME);  
  
  angle = ir.getAngle();
  intensity = ir.getIntensity(); Serial.print(intensity);  Serial.print(" ");
  distance = ir.getDistance(); Serial.println(distance);

  //bluetoothSignal = digitalRead(bluetooth);
  
  if(millis() > timer){
    timer = millis() + 5;
    uart.sendInfo(angle, intensity, distance);
    //Serial.println(bluetoothSignal);
  }

  //ir.printIR(angle, intensity, 100, true);
  //Serial.println(loopTime);
}