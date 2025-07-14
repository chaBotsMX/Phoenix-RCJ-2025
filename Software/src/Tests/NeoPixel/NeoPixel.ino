#include <Adafruit_NeoPixel.h>

#define neoPin 10
#define numPixels 18
Adafruit_NeoPixel pixels(numPixels, neoPin, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);

  pixels.begin();

  pixels.clear();

  for(int i = 0; i < numPixels; i++){
    pixels.setPixelColor(i, pixels.Color(50, 50, 50));
    pixels.show();
    delay(100);
  }

}

void loop() {
}
