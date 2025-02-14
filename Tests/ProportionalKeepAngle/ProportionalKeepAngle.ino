const int pwmFrontLeft = 9, pwmFrontRight = 6, pwmBackLeft = 29, pwmBackRight = 3;
const int frontLeftBackward = 28, frontLeftForward = 8, frontRightBackward = 7, frontRightForward = 5,
  backLeftBackward = 31, backLeftForward = 30, backRightBackward = 4, backRightForward = 2;

float yaw=0;

void setup() {
  pinMode(frontLeftBackward, OUTPUT);
  pinMode(frontLeftForward, OUTPUT);
  pinMode(frontRightBackward, OUTPUT);
  pinMode(frontRightForward, OUTPUT);
  pinMode(backLeftBackward, OUTPUT);
  pinMode(backLeftForward, OUTPUT);
  pinMode(backRightBackward, OUTPUT);
  pinMode(backRightForward, OUTPUT);

  pinMode(pwmFrontLeft, OUTPUT);
  pinMode(pwmFrontRight, OUTPUT);
  pinMode(pwmBackLeft, OUTPUT);
  pinMode(pwmBackRight, OUTPUT);

  Serial.begin(115200);  // serial monitor
  Serial1.begin(9600);   // uart
  Serial.println("waiting for data");
}

void loop() {
  getYawData();
  keepAngle(0, 150, 0.1);
  //frontLeftPower(150, 0, 1);
  //frontRightPower(150, 1, 0);
  //backLeftPower(150, 0, 1);
  //backRightPower(150, 1, 0);
}

void keepAngle (float targetAngle, int maxPwm, double kP) {
  float currentAngle = yaw;
  float error = targetAngle - currentAngle;
  int pwm = (int) (kP*error);
  pwm = constrain(pwm, -maxPwm, maxPwm);

  /*frontLeftPower(pwm, 0, 1);
  frontRightPower(pwm, 0, 1);
  backLeftPower(pwm, 0, 1);
  backRightPower(pwm, 0, 1);*/

  frontLeftPower(pwm, pwm >= 0 ? 0 : 1, pwm >= 0 ? 1 : 0);
  frontRightPower(pwm, pwm >= 0 ? 0 : 1, pwm >= 0 ? 1 : 0);
  backLeftPower(pwm, pwm >= 0 ? 0 : 1, pwm >= 0 ? 1 : 0);
  backRightPower(pwm, pwm >= 0 ? 0 : 1, pwm >= 0 ? 1 : 0);
}

void getYawData(){
  static String receivedData = ""; 

  char incomingChar;

  while (Serial1.available()) {
    incomingChar = Serial1.read();

    if (incomingChar == '\n') {
      yaw = receivedData.toFloat();
      Serial.print("Yaw: ");
      Serial.println(yaw);
      receivedData = "";
    } else {
      receivedData += incomingChar;
    }
  }
}

void frontLeftPower(int pwm, int dir1, int dir2) {
  digitalWrite(frontLeftBackward, dir1); digitalWrite(frontLeftForward, dir2);
  analogWrite(pwmFrontLeft, pwm);
}

void frontRightPower(int pwm, int dir1, int dir2) {
  digitalWrite(frontRightBackward, dir1); digitalWrite(frontRightForward, dir2);
  analogWrite(pwmFrontRight, pwm);
}

void backLeftPower(int pwm, int dir1, int dir2) {
  digitalWrite(backLeftBackward, dir1); digitalWrite(backLeftForward, dir2);
  analogWrite(pwmBackLeft, pwm);
}

void backRightPower(int pwm, int dir1, int dir2) {
  digitalWrite(backRightBackward, dir1); digitalWrite(backRightForward, dir2);
  analogWrite(pwmBackRight, pwm);
}