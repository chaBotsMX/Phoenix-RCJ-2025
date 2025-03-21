#define numIR 18
#define timeInter 5000

const int ir[numIR] = {33, 34, 36, 37, 38, 19, 20, 22, 23, 0, 1, 3, 4, 27, 28, 29, 31, 32};
bool readings[numIR];
unsigned long endRead;

void setup() {
  Serial.begin(115200);
  Serial.println("Prueba TSSPS");

  for (int i = 0; i < numIR; i++) {
    pinMode(ir[i], INPUT);
  }

  endRead = micros() + timeInter;

  delay(500);
}

void loop() {
  if(micros() > endRead){
    for (int i = 0; i < numIR; i++) {
      Serial.print(readings[i]);

      if (i < numIR - 1) {
        Serial.print(' ');
      }

      readings[i] = false;
    }
    Serial.print('\n');

    endRead = micros() + timeInter;
  } else{
    for(int i = 0; i< numIR; i++) {
      readings[i] |= !digitalRead(ir[i]);
    }
  }
}