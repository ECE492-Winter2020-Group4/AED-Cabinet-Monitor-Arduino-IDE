#pragma once

// GPIO PINS
#define GPIO_DETECTOR 15
#define GPIO_LED 5

// EEPROM VARIABLES
#define EEPROM_SIZE 256
#define MODE_ADDRESS 0
#define REQUEST_ADDRESS 2
#define CONFIG_ADDRESS 10

// REQUEST VARIABLES
#define NO_REQUEST 0
#define SLEEP_REQUEST 1
#define EMAIL_REQUEST 2
#define CONFIG_REQUEST 3

// MODE OF OPERATION VARIABLES
#define REGULAR_MODE 0
#define BLE_MODE 1

// EMAIL AND DEEP SLEEP CONSTANTS
#define DEEP_SLEEP_FOR_X_SECONDS 60
#define STAY_AWAKE_FOR_X_SECONDS 10
#define MAX_WAIT_TIME_SECONDS 30
#define MAX_WIFI_RECONNECT_ATTEMPTS 4
#define MAX_EMAIL_RESEND_ATTEMPTS 4

// BLUETOOTH UUID VARIABLES
#define SERVICE_UUID "0434f706-7af9-4349-8de8-701c14119b5a"
#define CHARACTERISTIC_UUID "d1b6c2fe-b2d4-462d-9509-04d745b79d30"

// WIFI VARIABLES
#define WIFI_SSID "UWS"        // wifi ssid
#define WIFI_PASSWORD "******" // wifi password for home wifi only

/* WPA2 enterprise only */
#define EAP_ANONYMOUS_IDENTITY "anonymous"
#define EAP_IDENTITY "******" // ualberta ccid
#define EAP_PASSWORD "******" // ualberta password
/************************/
enum WifiType
{
    HOME_WIFI = 0,
    ENTERPRISE_WIFI = 1
};
#define WIFI_TYPE ENTERPRISE_WIFI // select between enterprise and home

#define SENDER_GMAIL_ADDRESS "uaececapstone@gmail.com" // sender's gmail address
#define SENDER_GMAIL_PASSWORD "******"                 // sender's gmail address
#define RECEIVER_EMAIL_ADDRESS "******"                // receiver's gmail address

// DEVICE VARIABLES
#define MODULE "1"                 // default module
#define LOCATION "UAlberta Campus" // default location