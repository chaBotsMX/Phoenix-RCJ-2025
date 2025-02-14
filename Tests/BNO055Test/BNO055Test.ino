#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup() {
  Serial.begin(9600);

  if (!bno.begin()) {
    Serial.println("no BNO055 detected");
    while (1);
  }

  bno.setExtCrystalUse(true);
}

void loop() {
  sensors_event_t event;
  bno.getEvent(&event);

  Serial.println(event.orientation.x); //send yaw via uart

  delay(10);
}
