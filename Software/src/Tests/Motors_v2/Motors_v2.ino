const int pwmPins[4] = {37, 25, 15, 10};
const int in1Pins[4] = {38, 26, 16, 11};
const int in2Pins[4] = {36, 24, 14, 9};

void setup(){
  for (int i = 0; i < 4; i++) {
    pinMode(pwmPins[i], OUTPUT);
    analogWriteFrequency(pwmPins[i], 20000);
    pinMode(in1Pins[i], OUTPUT);
    pinMode(in2Pins[i], OUTPUT);
  }
}

void setAllMotorsOutput(int power){
  power = constrain(power, -255, 255);

  for(int i = 0; i < 4; i++){
    analogWrite(pwmPins[i], abs(power));
    digitalWrite(in2Pins[i], power > 0);
    digitalWrite(in1Pins[i], power < 0);
  }
}

void loop(){
  setAllMotorsOutput(100);
}