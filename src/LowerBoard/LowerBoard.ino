#include "LineSensor.h"

LineSensor ls;

void setup() {
  ls.begin();
}

void loop() {
  ls.update();
}
