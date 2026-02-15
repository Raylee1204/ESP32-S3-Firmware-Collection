#include <BLEDevice.h>

// 要搜尋的藍芽服務UUID
static BLEUUID serviceUUID("2b954f7a-c2ad-4874-8920-e765d5afafe2");

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        Serial.print("Found our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        advertisedDevice.getScan()->stop();
        _device = new BLEAdvertisedDevice(advertisedDevice);
      }
    }
    
    BLEAdvertisedDevice* getDevice() {
      return _device;
    }

private:
    BLEAdvertisedDevice* _device;
};

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
  }
};

void setup() {
  Serial.begin(115200);

  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  MyAdvertisedDeviceCallbacks* pMyAdvertisedDeviceCallbacks = new MyAdvertisedDeviceCallbacks();
  pBLEScan->setAdvertisedDeviceCallbacks(pMyAdvertisedDeviceCallbacks);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5);

  BLEAdvertisedDevice* foundDevice = nullptr;
  while (foundDevice == nullptr) {
    delay(100);
    foundDevice = pMyAdvertisedDeviceCallbacks->getDevice();
  }

  BLEClient* pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());

  // 連接到Master端
  pClient->connect(foundDevice);

  // 搜尋藍芽服務
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);

  if (pRemoteService == nullptr) {
    Serial.println("Failed to find our service UUID");
    
    pClient->disconnect();
    return;
  }

  // 搜尋藍芽特徵
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(BLEUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8"));

  if (pRemoteCharacteristic == nullptr) {
    BLEUUID characteristicUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");
    Serial.println("Failed to find our characteristic UUID");

    pClient->disconnect();
    return;
  }

  // 讀取藍芽特徵的數值
  String value = String(pRemoteCharacteristic->readValue().c_str());


  Serial.print("The value of the characteristic is: ");
  Serial.println(value);


  pClient->disconnect();
}

void loop() {
  // 因為要觀察Client Disconnect的狀態，故讓它只執行一次
}
