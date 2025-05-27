#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

//int receivedValue = -1;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    
    //if (value.length() > 0) {
    int receivedValue = (uint8_t)value[0] - '0';  // Read the first byte as int
    Serial.print("Received value: "); Serial.println(receivedValue);

    switch (receivedValue) {
      case 1: Serial.println("Left"); Serial1.write(1); break;
      case 2: Serial.println("Right"); Serial1.write(2); break;
      case 3: Serial.println("Up"); Serial1.write(3); break;
      case 4: Serial.println("Down"); Serial1.write(4); break;
      case 5: Serial.println("Action"); Serial1.write(5); break;
      default: Serial.println("Unknown command"); Serial1.write(0);
    }
    //}
  }
};

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 18, 17);
  delay(1000);

  BLEDevice::init("T4S3_Peripheral");

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE Peripheral ready.");
}

void loop() {
  if (deviceConnected) {
    Serial.println("Connected");
    delay(100);
  } else{
    Serial.println("Disconnected");
    delay(1000);
  }
}
