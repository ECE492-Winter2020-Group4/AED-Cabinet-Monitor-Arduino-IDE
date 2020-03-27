#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string.h>

#include "../include/defs.hpp"
#include "../include/BluetoothHandler.hpp"
#include "EEPROM.h"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

String getMessageString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void writeConfig(String data)
{
    int data_size = data.length();
    for (int i = 0; i < data_size; i++)
    {
        EEPROM.write(CONFIG_ADDRESS + i, data[i]);
    }
    EEPROM.write(CONFIG_ADDRESS + data_size, '\0');
    EEPROM.commit();
}

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};

class FlutterAEDCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0)
        {

            if (strcmp("SLEEP", value.c_str()) == 0)
            {
                EEPROM.write(REQUEST_ADDRESS, SLEEP_REQUEST);
                EEPROM.commit();
                Serial.println("Sleep Request from device");
            }
            else if (strcmp("EMAIL", value.c_str()) == 0)
            {
                EEPROM.write(REQUEST_ADDRESS, EMAIL_REQUEST);
                EEPROM.commit();
                Serial.println("Email Request from device");
            }
            else
            {
                EEPROM.write(REQUEST_ADDRESS, CONFIG_REQUEST);
                EEPROM.commit();

                String module = getMessageString(value.c_str(), ',', 0);
                String location = getMessageString(value.c_str(), ',', 1);
                Serial.printf("Received:\nModule: %s\nLocation: %s\n", module, location);
                // Modify config information
                writeConfig(value.c_str());
            }
            ESP.restart();
        }
    }
};

BluetoothHandler::BluetoothHandler()
{
    Serial.println("Creating BluetoothHandler");
}

void BluetoothHandler::initServer()
{
    pServer = NULL;
    pCharacteristic = NULL;
    deviceConnected = false;
    oldDeviceConnected = false;
    value = 0;
    // Create the BLE Device
    BLEDevice::init("AED-ESP32-Monitor-1");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_INDICATE);

    pCharacteristic->setCallbacks(new FlutterAEDCallbacks());
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting for a client connection to notify...");
}

void BluetoothHandler::run()
{
    Serial.println("Running BluetoothHandler");
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        oldDeviceConnected = deviceConnected;
    }
}