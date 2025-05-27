/*---APP---*/

#include "IMU.h"
#include "Motors.h"
#include "UART.h"
#include "PID.h"
#include "UI.h"
#include "Kicker.h"

const int motorsPWM = 55;

IMU imu;
Motors motors;
UART uart;
UI ui;
Kicker kicker;
elapsedMillis camCounter;
PID pid(1.85, 0.1, motorsPWM);
int setpointOffset = 0;
unsigned long long correctionUpdate = 0;
unsigned long printUpdate = 0;
unsigned long cameraUpdate = 0;

unsigned long kickTimer = 0;

int angleIR = 500;
int intensityIR = 0; const int maxIntensityIR = 2200;
int distanceIR = 1000;
int currentOffset = 0;
int angleLine = 500;

int blobX = -1, blobY = -1;

float setpoint = 0;
int correction = 0;
int timerAngle = 0;
bool firstDetected = false;
int firstSector = -1;

int angleCam = -1;
elapsedMillis timerLS;

bool kicked = false;

int receivedBLE = -1;

void setup() {
  Serial.begin(115200);
  uart.beginIR(115200);
  uart.beginLS(115200);
  //uart.beginCam(19200);
  uart.beginDisplay(9600);

  delay(1000);

  pinMode(LED_BUILTIN, OUTPUT);

  if (!imu.begin()) {
    Serial.println("imu not found");
    digitalWrite(LED_BUILTIN, HIGH);
    while (1);
  }
}

void loop() {
  ui.update();
  kicker.update();

  if(kicked && millis() - kickTimer > 5000){
    kicked = false;
  }

  if(ui.rightButtonToggle){
    receivedBLE = Serial1.read();
    Serial.println(receivedBLE);
    switch (receivedBLE){
      case 1: motors.setAllMotorsOutput(motorsPWM); setpoint = imu.getYaw(); break;
      case 2: motors.setAllMotorsOutput(-motorsPWM); setpoint = imu.getYaw(); break;
      case 3: motors.driveToAngle(0, motorsPWM, correction); break;
      case 4: motors.driveToAngle(180, motorsPWM, correction); break;
      case 5:
        if(!kicked){
          ui.buzz(500, 300);
          kicker.kick();
          kicked = true;
          kickTimer = millis();
        }
        break;
      default: motors.setAllMotorsOutput(0);
    }
  } else{
    motors.setAllMotorsOutput(0);
  }
  
  if (imu.update()) {
    float yaw = imu.getYaw(); //Serial.println(yaw);
    float error = yaw - setpoint;

    if (millis() > correctionUpdate) {
      correctionUpdate = millis() + 10;
      correction = pid.getCorrection(error);
    }
  }
}