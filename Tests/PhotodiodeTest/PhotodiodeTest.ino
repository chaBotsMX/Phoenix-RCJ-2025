#include <Adafruit_NeoPixel.h>

#define neoPixel 17
#define numNeoPixel 30
#define numDiodes 30
//const int diodes[numDiodes] = {0, 1, 2, 3, 4, 5, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41};
const int diodes[numDiodes] = {35, 34, 37, 33, 18, 19, 20, 21, 2, 3, 4, 5, 28, 32, 31, 30, 36, 38, 39, 40, 41, 23, 22, 0, 1, 24, 25, 27, 26};
Adafruit_NeoPixel pixels(numNeoPixel, neoPixel, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < numDiodes; i++){
    pinMode(diodes[i], INPUT);
  }

  pixels.begin();

  pixels.clear();

  for(int i = 0; i < numNeoPixel; i++){
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
    pixels.show();
    delay(100);
  }

}

void loop() {
  for (int i = 0; i < numDiodes; i++) {
    Serial.print(digitalRead(diodes[i]));
    if (i < numDiodes - 1) {
      Serial.print(' ');
    }
  }
  Serial.println();
  delay(100);
}
