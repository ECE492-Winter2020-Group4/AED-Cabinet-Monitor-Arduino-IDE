#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <string.h>

#include "../include/defs.hpp"
class BluetoothHandler
{
public:
    BluetoothHandler();
    void initServer();
    void run();

private:
};