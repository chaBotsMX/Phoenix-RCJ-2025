const int button = 6;

void setup() {
  Serial.begin(115200);
  pinMode(button, INPUT);
}

void loop() {
  Serial.println(digitalRead(button));
  delay(50);
}
