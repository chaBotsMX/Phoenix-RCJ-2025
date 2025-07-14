#define numIR 18
#define timeInter 10000
#define maxWidth 600

const int ir[numIR] = {33, 34, 36, 37, 38, 19, 20, 22, 23, 0, 1, 3, 4, 27, 28, 29, 31, 32};
bool readings[numIR];

unsigned long endRead;

unsigned long intensity[numIR];
unsigned long start[numIR];
unsigned long timeInZero[numIR];

bool current[numIR];
bool prev[numIR];

void setup() {
  Serial.begin(115200);
  Serial.println("Prueba TSSPS");

  for (int i = 0; i < numIR; i++) {
    pinMode(ir[i], INPUT);
  }

  endRead = micros() + timeInter;
}

void loop() {
  if(micros() > endRead){
    //Serial.println(intensity);
    for(unsigned long inst : intensity){
      Serial.print(inst);
      Serial.print('\t');
    }
    Serial.print('\n');

    endRead = micros() + timeInter;
  }

  for(int i = 0; i < numIR; i++){
    current[i] = !digitalRead(ir[i]);

    if(prev[i] == false && current[i] == true){
      start[i] = micros();
    }
    if(prev[i] ==true && current[i] == false){
      intensity[i] = micros() - start[i];
      timeInZero[i] = millis() + 5;
    }
    if(current[i] == false && millis() > timeInZero[i]){
      intensity[i] = 0;
    }
    prev[i] = current[i];
  }
}