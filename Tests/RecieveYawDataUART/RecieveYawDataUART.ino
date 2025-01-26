void setup() {
  Serial.begin(115200);  // Comunicación con el monitor serie
  Serial1.begin(9600);   // Comunicación UART con el Arduino UNO
  Serial.println("Esperando datos del Arduino UNO...");
}

void loop() {
  static String receivedData = "";  // Almacena temporalmente los datos recibidos
  char incomingChar;

  // Verifica si hay datos disponibles en el puerto UART
  while (Serial1.available()) {
    incomingChar = Serial1.read();  // Lee carácter por carácter

    // Si detecta el final de línea (\n), procesa los datos
    if (incomingChar == '\n') {
      float yaw = receivedData.toFloat();  // Convierte el texto a float
      Serial.print("Yaw recibido: ");
      Serial.println(yaw);  // Imprime el valor convertido
      receivedData = "";  // Resetea la cadena para el próximo dato
    } else {
      receivedData += incomingChar;  // Construye la cadena de datos
    }
  }
}
