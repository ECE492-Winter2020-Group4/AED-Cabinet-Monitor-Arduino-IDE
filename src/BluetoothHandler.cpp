
#include "../include/BluetoothHandler.hpp"
#include "../include/EEPROMHelper.hpp"

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

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
                writeToEEPROM(REQUEST_ADDRESS, SLEEP_REQUEST);
                Serial.println("Sleep Request from device");
            }
            else if (strcmp("EMAIL", value.c_str()) == 0)
            {
                writeToEEPROM(REQUEST_ADDRESS, EMAIL_REQUEST);
                Serial.println("Email Request from device");
            }
            else
            {
                writeToEEPROM(REQUEST_ADDRESS, CONFIG_REQUEST);

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
    char deviceName[50];

    // Read Configuration data from EEPROM
    String config = readConfigData();

    // Get module from config data
    String module = getMessageString(config, ',', 0);

    // If nothing in config data, use default device config
    if(module.length() == 0){
        module = MODULE;
    }

    // Create device name based on module
    sprintf(deviceName, "AED-ESP32-Monitor-%s", module);

    // Create the BLE Device
    BLEDevice::init(deviceName);

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

    // Create callback from mobile application Characteristic
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