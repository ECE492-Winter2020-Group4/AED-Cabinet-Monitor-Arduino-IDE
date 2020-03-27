#include <Arduino.h>
#include "../include/WakeupHandler.hpp"
#include "../include/defs.hpp"
#include "../include/EmailHandler.hpp"
#include "../include/BluetoothHandler.hpp"
#include "../include/EEPROMHelper.hpp"

WifiHandler h_wifi;
EmailHandler h_email;
BluetoothHandler h_bluetooth;
int mode_index;
int request;
String configData;



WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");
    // Inititalize Button Reading
    initEEPROM();

    // Set index to button state
    mode_index = readEEPROMValue(MODE_ADDRESS);

    request = readEEPROMValue(REQUEST_ADDRESS);

    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);
    h_bluetooth = BluetoothHandler();
    bool connect_test, send_test = false;

    switch (request)
    {
    // Detected door opening
    case SLEEP_REQUEST:
        Serial.println("Request to return to sleep mode");
        break;
    case EMAIL_REQUEST:
        Serial.println("Test Email Requested");
        // connect to wifi and send email alert
        // repeat again if connected to wifi but failed to send alert
        for (int i = 0; i < 2 && connect_test && !send_test; i++)
        {
            Serial.print("Round ");
            Serial.println(i + 1);

            // connect to wifi
            h_wifi.initConnection();
            connect_test = h_wifi.getConnectionStatus();
            // send test email
            if (connect_test)
            {
                delay(500); // wait for 0.5 sec before sending email
                h_email.sendTestMsg();
                send_test = h_email.getEmailStatus();
            }
            h_wifi.closeConnection();
        }

        mode_index = 1;
        
        break;
    case CONFIG_REQUEST:
        Serial.println("Configuration Requested");
        configData = readConfigData();
        if (configData.length() > 0)
        {
            String module = getMessageString(configData, ',', 0);
            String location = getMessageString(configData, ',', 1);
            Serial.printf("Module: %s\nLocation: %s\n", module, location);
        }
        mode_index = 1;

        break;
    default:
        Serial.println("No Requests");
    }

    writeToEEPROM(REQUEST_ADDRESS, NO_REQUEST);
    writeToEEPROM(MODE_ADDRESS, mode_index != 0 ? 0 : 1);
    
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