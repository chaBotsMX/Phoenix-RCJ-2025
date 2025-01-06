const int pwmFrontLeft = 9, pwmFrontRight = 6, pwmBackLeft = 29, pwmBackRight = 3;
const int frontLeftBackward = 28, frontLeftForward = 8, frontRightBackward = 7, frontRightForward = 5,
  backLeftBackward = 31, backLeftForward = 30, backRightBackward = 4, backRightForward = 2;

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
}

void loop() {
  frontLeftPower(150, 0, 1);
  frontRightPower(150, 1, 0);
  backLeftPower(150, 0, 1);
  backRightPower(150, 1, 0);
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