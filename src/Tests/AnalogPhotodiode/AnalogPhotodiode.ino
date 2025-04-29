#include <Adafruit_NeoPixel.h>

#define neoPin 33
#define numSensors 18
const int diodes[numSensors] = {A1, A0, A14, A15, A16, A17, A4, A5, A6, A9, A8, A7, A10, A11, A12, A13, A3, A2};
Adafruit_NeoPixel pixels(numSensors, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  analogReadResolution(8);

  pixels.begin();

  pixels.clear();

  for(int i = 0; i < numSensors; i++){
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(100);
  }

}

void loop() {
  for (int i = 0; i < numSensors; i++) {
    Serial.print(analogRead(diodes[i]));
    if (i < numSensors - 1) {
      Serial.print(' ');
    }
  }
  Serial.println();
  delay(10);
}
