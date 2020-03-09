#pragma once
#define GPIO_DETECTOR 15
#define GPIO_LED 5

#define DEEP_SLEEP_FOR_X_SECONDS 60
#define STAY_AWAKE_FOR_X_SECONDS 15

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
