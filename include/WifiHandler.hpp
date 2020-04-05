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
    void closeConnection();
    bool getConnectionStatus();
    String getLocalIPAdress();

private:
    uint8_t connection_state = 0;                                  // Connected to WIFI or not
    uint16_t connect_wait_time = WIFI_CONNECT_WAIT_TIME_SECONDS;   // Max wait time for wifi connection status
    uint16_t max_reconnect_attempts = MAX_WIFI_RECONNECT_ATTEMPTS; // Max # of attempts for reconnecting to wifi
    String local_ip_address = "";
    uint8_t wifiConnect();
    void reconnect();
};