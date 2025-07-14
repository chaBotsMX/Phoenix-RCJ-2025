#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEClient.h>
#include <BLEScan.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Found device: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(SERVICE_UUID))) {
      Serial.println("Found matching server!");
      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;
      advertisedDevice.getScan()->stop();
    }
  }
};

bool connectToServer() {
  BLEClient* pClient = BLEDevice::createClient();
  if (!pClient->connect(*pServerAddress)) return false;

  BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
  if (pRemoteService == nullptr) return false;

  pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
  if (pRemoteCharacteristic == nullptr) return false;

  return true;
}

void setup() {
  Serial.begin(115200);
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5);
}

void loop() {
  //Serial.println("debug");
  if (doConnect) {
    if (connectToServer()) {
      String value = pRemoteCharacteristic->readValue().c_str();
      Serial.println("Received: " + value);
    } else {
      Serial.println("Failed to connect");
    }
    doConnect = false;
  }

  delay(1000);
}