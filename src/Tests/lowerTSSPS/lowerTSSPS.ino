const int ir[4] = {19, 20, 21, 22};

void setup() {
  for(int i = 0; i < 4; i++){
    pinMode(ir[i], INPUT);
  }
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 4; i++){
    Serial.print(digitalRead(ir[i])); Serial.print('\t');
  }
  Serial.println();

  delay(100);
}
