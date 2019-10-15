
/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
 
   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8
 
   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.
 
   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
 
BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t value = 0;
 
bool ledFlg = false;
 
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
 
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
 
 
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
 
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};
 
 
 
void setup() {
  Serial.begin(115200);
 
  pinMode(14, OUTPUT);  
 
  // Create the BLE Device
  BLEDevice::init("MyESP32");
 
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
 
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
 
  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );
 
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());
 
  // Start the service
  pService->start();
 
  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
 
}
 
void loop() {
 
  if (deviceConnected) {
    int t0val  = touchRead(T0);  // get value using T0 - IO04
    int t3val  = touchRead(T3);  // get value using T3 - IO15
    bool flg = false;

      Serial.println("\n\rT0");
      Serial.println(t0val);
      Serial.println("\n\rT3");
      Serial.println(t3val);

    if(t0val < 30 && !ledFlg){
      //switch on
      digitalWrite(14,HIGH);
      flg = true;
      ledFlg = true;
      value = 1;
      Serial.println("*** SWITCH ON ***");
    }
    if(t3val < 30 && ledFlg){
      //switch off
      digitalWrite(14,LOW);
      flg = true;
      ledFlg = false;
      value = 0;
      Serial.println("*** SWITCH OFF ***");
    }
 
    if(flg){
      pCharacteristic->setValue(&value, 1);
      pCharacteristic->notify();
      //pCharacteristic->indicate();
      //value++;
    }
 
  }
  delay(2000);
}
 

