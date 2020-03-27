#include <Arduino.h>
#include "../include/WakeupHandler.hpp"
#include "../include/defs.hpp"
#include "../include/EmailHandler.hpp"
#include "../include/BluetoothHandler.hpp"
#include "EEPROM.h"

WifiHandler h_wifi;
EmailHandler h_email;
BluetoothHandler h_bluetooth;
int mode_index;
int request;
String configData;

String readConfigData()
{
    char data[100];
    int len = 0;
    unsigned char k;
    k = EEPROM.read(CONFIG_ADDRESS);
    while (k != '\0' && len < 500)
    {
        k = EEPROM.read(CONFIG_ADDRESS + len);
        data[len] = k;
        len++;
    }
    data[len] = '\0';
    return String(data);
}

WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");
    // Inititalize Button Reading
    if (!EEPROM.begin(EEPROM_SIZE))
    {
        delay(1000);
    }

    // Set index to button state
    mode_index = EEPROM.read(MODE_ADDRESS);

    request = EEPROM.read(REQUEST_ADDRESS);

    Serial.printf("mode_index: %d\n request: %d\n", mode_index, request);
    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);
    h_bluetooth = BluetoothHandler();

    switch (request)
    {
    // Detected door opening
    case SLEEP_REQUEST:
        Serial.println("Request to return to sleep mode");
        break;
    case EMAIL_REQUEST:
        Serial.println("Test Email Requested");
        mode_index = 1;
        break;
    case CONFIG_REQUEST:
        Serial.println("Configuration Requested");
        configData = readConfigData();
        Serial.println(configData);
        // if (configData.length() > 0)
        // {
        //     String module = getMessageString(configData, ',', 0);
        //     String location = getMessageString(configData, ',', 1);

        //     if (module.length() > 0 && location.length() > 0)
        //     {
        //         Serial.print("module : ");
        //         Serial.println(module);

        //         Serial.print("location : ");
        //         Serial.println(location);
        //     }
        // }
        mode_index = 1;

        break;
    default:
        Serial.println("No Requests");
    }

    EEPROM.write(MODE_ADDRESS, mode_index != 0 ? 0 : 1);
    EEPROM.commit();
}

void WakeupHandler::handle()
{
    bool connect_success, send_success = false;
    switch (esp_sleep_get_wakeup_cause())
    {
    // Detected door opening
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("ISR0 Wakeup");
        // connect to wifi and send email alert
        // repeat again if connected to wifi but failed to send alert
        for (int i = 0; i < 2 && connect_success && !send_success; i++)
        {
            Serial.print("Round ");
            Serial.println(i + 1);

            // connect to wifi
            h_wifi.initConnection();
            connect_success = h_wifi.getConnectionStatus();
            // send open door alert email
            if (connect_success)
            {
                delay(500); // wait for 0.5 sec before sending email
                h_email.sendOpenDoorAlert();
                send_success = h_email.getEmailStatus();
            }
            h_wifi.closeConnection();
        }
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Timer Wakeup");
        // TODO : LOG
        break;
    default:
        // Boot up or reset triggered
        if (mode_index == 1)
        {
            Serial.println("Bluetooth Server Mode");
            h_bluetooth.initServer();

            // Busy wait
            while (mode_index)
            {
            }
        }
        else
        {
            Serial.println("Sleep Mode");
            break;
        }
    }
}