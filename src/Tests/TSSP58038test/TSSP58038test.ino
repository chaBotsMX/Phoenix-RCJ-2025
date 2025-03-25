#define numIR 18
const int ir[numIR] = {33, 34, 36, 37, 38, 19, 20, 22, 23, 0, 1, 3, 4, 27, 28, 29, 31, 32};

void setup() {
  Serial.begin(115200);
  Serial.println("Prueba TSSPS");

  for (int i = 0; i < numIR; i++) {
    pinMode(ir[i], INPUT);
  } 

  delay(500);
}

void loop() {
  for (int i = 0; i < numIR; i++) {
    Serial.print(digitalRead(ir[i]));
    if (i < numIR - 1) {
      Serial.print(' ');
    }
  }
  Serial.println();
  delay(100);
}