const int ir[4] = {19, 20, 21, 22};
#define numIR 4
bool detected[numIR], lastDetected[numIR];
unsigned long pulseWidth[numIR], timeInHigh[numIR], timeInLow[numIR];

const float vectorX[numIR] = {
  0.7071067811,  -0.7071067811,  -0.7071067811,  0.7071067811
};
const float vectorY[numIR] = {
  0.7071067811, 0.7071067811, -0.7071067811, -0.7071067811
};

int intensity = -1;
void setup() {
  for(int i = 0; i < 4; i++){
    pinMode(ir[i], INPUT);
  }
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*for(int i = 0; i < 4; i++){
    Serial.print(digitalRead(ir[i])); Serial.print('\t');
  }
  Serial.println();

  delay(100);*/
  unsigned long lastUpdate = 0;

  if((micros() - lastUpdate) > 833){
    lastUpdate = micros();

    unsigned long currentMicros = micros();
    unsigned long currentMillis = millis();

    for(int i = 0; i < numIR; i++){
      detected[i] = !digitalReadFast(ir[i]); //get current readings as booleans

      if(!lastDetected[i] && detected[i]){
        timeInHigh[i] = currentMicros; //start counting time in HIGH
      }
      if(lastDetected[i] && !detected[i]){
        pulseWidth[i] = currentMicros - timeInHigh[i]; //stop counting time, save in array
        timeInLow[i] = currentMillis + 5;
      }

      if(!lastDetected[i] && currentMillis > timeInLow[i]){
        pulseWidth[i] = 0;
      }

      lastDetected[i] = detected[i];
    }

    float sumX = 0, sumY = 0;
    int sensorsReading = 0;

    for(int i = 0; i < numIR; i++){
      if(pulseWidth[i] > 0){
        sumX += pulseWidth[i] * vectorX[i];
        sumY += pulseWidth[i] * vectorY[i];
        sensorsReading++;
      }
    }

    intensity = sqrt((sumX * sumX) + (sumY * sumY));
    //if(intensity > maxIntensity) intensity = maxIntensity;
  }

  //for(int i = 0; i < numIR; i++){
    //Serial.print(pulseWidth[i]); Serial.print('\t');
  //}
  Serial.println(intensity);
}
