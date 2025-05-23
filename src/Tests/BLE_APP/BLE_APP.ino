#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;

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
      Serial.print("Received value: ");
      Serial.println(receivedValue);

      // You can use a switch-case here for control logic
      switch (receivedValue) {
        case 1: Serial.println("Left"); break;
        case 2: Serial.println("Right"); break;
        case 3: Serial.println("Up"); break;
        case 4: Serial.println("Down"); break;
        case 5: Serial.println("Action"); break;
        default: Serial.println("Unknown command");
      }
    //}
  }
};

void setup() {
  Serial.begin(115200);
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
    delay(1000);
  } else{
    Serial.println("Disconnected");
    delay(1000);
  }
}
