void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial.println("waiting for data");
}

void loop() {
  static String receivedData = "";
  char incomingChar;

  while (Serial1.available()) {
    incomingChar = Serial1.read();

    if (incomingChar == '\n') {
      float yaw = receivedData.toFloat();
      Serial.print("Received yaw: ");
      Serial.println(yaw);
      receivedData = "";
    } else {
      receivedData += incomingChar;
    }
  }
}
