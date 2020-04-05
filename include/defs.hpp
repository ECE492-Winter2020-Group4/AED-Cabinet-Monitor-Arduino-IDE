#pragma once

// See Application Notes for instructions on how to fill this out

/////////////////////////////////////////////////////////////////
// Connection Settings                                         //
/////////////////////////////////////////////////////////////////

// Email Message Content
#define MODULE "1"          // default module
#define LOCATION "UAlberta" // default location

// Wifi & Email Time Settings
#define WIFI_CONNECT_WAIT_TIME_SECONDS 30 // wait time for connection status after attempting to connect to wifi
#define MAX_WIFI_RECONNECT_ATTEMPTS 4     // max # of attempts to reconnect to wifi
#define EMAIL_SEND_WAIT_TIME_SECONDS 3    // wait time for send status after attempting to send an email
#define MAX_EMAIL_RESEND_ATTEMPTS 4       // max # of attempts to resend the email

// WiFi Constants
enum WifiType
{
    HOME_WIFI = 0,
    ENTERPRISE_WIFI = 1
};

// WiFi Network Settings
#define WIFI_TYPE ENTERPRISE_WIFI // select between ENTERPRISE_WIFI and HOME_WIFI
#define WIFI_SSID "UWS"           // wifi ssid
#define WIFI_PASSWORD "******"    // wifi password (Only used when HOME_WIFI is selected)

// Only for WPA2 Enterprise Networks
#define EAP_ANONYMOUS_IDENTITY "anonymous" // (Only used when ENTERPRISE_WIFI is selected)
#define EAP_IDENTITY "******"              // ualberta ccid                  // (Only used when ENTERPRISE_WIFI is selected)
#define EAP_PASSWORD "******"              // ualberta password              // (Only used when ENTERPRISE_WIFI is selected)

// Email Account Settings
#define SENDER_GMAIL_ADDRESS "uaececapstone@gmail.com" // sender's gmail address
#define SENDER_GMAIL_PASSWORD "******"                 // sender's gmail address
#define RECEIVER_EMAIL_ADDRESS "******"                // receiver's gmail address

// Bluetooth UUID Settings
#define SERVICE_UUID "0434f706-7af9-4349-8de8-701c14119b5a"
#define CHARACTERISTIC_UUID "d1b6c2fe-b2d4-462d-9509-04d745b79d30"

/////////////////////////////////////////////////////////////////
// Hardware Settings                                           //
/////////////////////////////////////////////////////////////////

// Gpio Settings
#define GPIO_DETECTOR 15
#define GPIO_LED 5 // default to onboard LED

// Deep Sleep Settings
#define DEEP_SLEEP_FOR_X_SECONDS 60
#define STAY_AWAKE_FOR_X_SECONDS 10

/////////////////////////////////////////////////////////////////
// Internal Settings                                           //
/////////////////////////////////////////////////////////////////

// EEPROM Variables
#define EEPROM_SIZE 256
#define MODE_ADDRESS 0
#define REQUEST_ADDRESS 2
#define CONFIG_ADDRESS 10

// Request Variables
#define NO_REQUEST 0
#define SLEEP_REQUEST 1
#define EMAIL_REQUEST 2
#define CONFIG_REQUEST 3

// Mode of Operation Variables
#define REGULAR_MODE 0
#define BLE_MODE 1
