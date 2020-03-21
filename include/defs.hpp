#pragma once
#define GPIO_DETECTOR 15
#define GPIO_LED 5

#define DEEP_SLEEP_FOR_X_SECONDS 60
#define STAY_AWAKE_FOR_X_SECONDS 10
#define MAX_WIFI_RECONNECT_ATTEMPTS 4
#define MAX_EMAIL_RESEND_ATTEMPTS 4

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define MODE_ADDRESS 0

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

#define MODULE "1"                 // default module
#define LOCATION "UAlberta Campus" // default location
