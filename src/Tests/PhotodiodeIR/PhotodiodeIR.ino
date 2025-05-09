const int ad1 = A12, ad2 = A15;
float ema = 0;
float alpha = 0.3;
int readings;

void setup() {
  Serial.begin(9600);
  //ema = analogRead(ad2);
}

void loop() {
  readings = analogRead(ad2);
  ema = alpha * readings + (1 - alpha) * ema;

  Serial.print(analogRead(ad1)); Serial.print('\t');
  Serial.print(analogRead(ad2)); Serial.print('\n');
  //Serial.print(readings); Serial.print('\t');
  //Serial.println(ema);
  delay(50);
}
