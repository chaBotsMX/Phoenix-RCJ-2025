#include <Adafruit_BNO08x.h>

#define pi 3.1416
#define MA 1
#define MB 2
#define MC 3
#define MD 4
#define BNO08X_CS 10
#define BNO08X_INT 14
#define BNO08X_RESET 15

int PWMA = 37, PWMB = 25, PWMC = 15, PWMD = 10;
int MA1 = 38, MA2 = 36, MB1 = 26, MB2 = 24, MC1 = 16, MC2 = 14, MD1 =11, MD2 = 9;
int dirMov = 180;
int POW (570);
int dir=0;
long long nextUpdateDir=0;
double lastError = 0;

Adafruit_BNO08x  bno08x(BNO08X_RESET);
sh2_SensorValue_t sensorValue;

struct euler_t {
  float yaw;
  float pitch;
  float roll;
} ypr;

void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {

    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    if (degrees) {
      ypr->yaw *= RAD_TO_DEG;
    }
}
void motorSet(int ID, int POW)
  {
    if(ID==MA){
      analogWrite(PWMA,abs(POW));
      digitalWrite(MA1, POW>0);digitalWrite(MA2, POW<0); 
    }
    else if(ID==MB){
      analogWrite(PWMB,abs(POW));
      digitalWrite(MB1, POW>0);digitalWrite(MB2, POW<0); 
    }
    else if(ID==MC){
      analogWrite(PWMC,abs(POW));
      digitalWrite(MC1, POW>0);digitalWrite(MC2, POW<0); 
    }
    else if(ID==MD){
      analogWrite(PWMD,abs(POW));
      digitalWrite(MD1, POW>0);digitalWrite(MD2, POW<0); 
    }
  }
void setup(void) {
  analogWriteFrequency(23, 20000);
  analogWriteFrequency(22, 20000);
  analogWriteFrequency(19, 20000);
  analogWriteFrequency(18, 20000);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  pinMode(MD1, OUTPUT);
  pinMode(MD2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(PWMC, OUTPUT);
  pinMode(PWMD, OUTPUT);

  Serial.begin(115200);
  //while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit BNO08x test!");

  // Try to initialize!
  //if (!bno08x.begin_I2C()) {
  if (!bno08x.begin_UART(&Serial2)) {  // Requires a device with > 300 byte UART buffer!
  //if (!bno08x.begin_SPI(BNO08X_CS, BNO08X_INT)) {
    Serial.println("Failed to find BNO08x chip");
    while (1) { delay(10); }
  }
  Serial.println("BNO08x Found!");

  for (int n = 0; n < bno08x.prodIds.numEntries; n++) {
    Serial.print("Part ");
    Serial.print(bno08x.prodIds.entry[n].swPartNumber);
    Serial.print(": Version :");
    Serial.print(bno08x.prodIds.entry[n].swVersionMajor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionMinor);
    Serial.print(".");
    Serial.print(bno08x.prodIds.entry[n].swVersionPatch);
    Serial.print(" Build ");
    Serial.println(bno08x.prodIds.entry[n].swBuildNumber);
  }

  setReports();

  Serial.println("Reading events");
  delay(100);
}

// Here is where you define the sensor outputs you want to receive
void setReports(void) {
  Serial.println("Setting desired reports");
  if (! bno08x.enableReport(SH2_GAME_ROTATION_VECTOR)) {
    Serial.println("Could not enable game vector");
  }
}

void loop() {
  delay(10);

  if (bno08x.wasReset()) {
    Serial.print("sensor was reset ");
    setReports();
  }
  
  if (! bno08x.getSensorEvent(&sensorValue)) {
    return;
  }
  
  switch (sensorValue.sensorId) {
    
    case SH2_GAME_ROTATION_VECTOR:
      Serial.print("Game Rotation Vector - r: ");
      Serial.print(sensorValue.un.gameRotationVector.real);
      Serial.print(" i: ");
      Serial.print(sensorValue.un.gameRotationVector.i);
      Serial.print(" j: ");
      Serial.print(sensorValue.un.gameRotationVector.j);
      Serial.print(" k: ");
      Serial.print(sensorValue.un.gameRotationVector.k);
      Serial.print(" yaw: ");
      quaternionToEuler(sensorValue.un.gameRotationVector.real, sensorValue.un.gameRotationVector.i, sensorValue.un.gameRotationVector.j, sensorValue.un.gameRotationVector.k, &ypr, true);
      Serial.println(ypr.yaw);
      break;

  }
  if(millis() > nextUpdateDir){
      nextUpdateDir=millis()+10;
      double error = ypr.yaw;
      double KP = -POW / 180.0;
      double KD = -0.12;
      int P = (int)(KP * error);
      int D = (int)(KD * (error - lastError)/0.01);
      int PD = P + D;
      PD = max(min(PD,100),-100);
      lastError = error;
      int POWA = PD;
      int POWB = PD;
      int POWC = PD;
      int POWD = PD;
      motorSet(MA,POWA);
      motorSet(MB,POWB);
      motorSet(MC,POWC);
      motorSet(MD,POWD);
      Serial.print("0:");
      Serial.print(0);
      Serial.print(",");
      Serial.print("P:");
      Serial.print(P);
      Serial.print(",");
      Serial.print("D:");
      Serial.print(D);
  }
}