#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_wpa2.h"
#include "defs.hpp"

class WifiHandler
{
public:
    WifiHandler();
    void initConnection();
    bool getConnectionStatus();
    String getLocalIPAdress();
private:
    uint8_t connection_state = 0;         // Connected to WIFI or not
    uint16_t reconnect_interval = 10000;  // If not connected wait time to try again
    uint16_t max_reconnect_attempts = 2;  // Attempt to reconnect for max of 2 times
    String local_ip_address = "";
    uint8_t wifiConnect();
    void awaits();
};
