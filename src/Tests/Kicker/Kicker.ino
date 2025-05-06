const int sol = 32;

void setup() {
  pinMode(sol, OUTPUT);
}

void loop() {
  digitalWrite(sol, HIGH);
  delay(1000);
  digitalWrite(sol, LOW);
  delay(2000);
}
