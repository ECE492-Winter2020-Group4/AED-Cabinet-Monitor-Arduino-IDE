#include <Arduino.h>

#include "../include/BluetoothHandler.hpp"
#include "../include/defs.hpp"
#include "../include/EEPROMHelper.hpp"
#include "../include/EmailHandler.hpp"
#include "../include/GpioHandler.hpp"
#include "../include/WakeupHandler.hpp"

BluetoothHandler h_bluetooth;
EmailHandler h_email;
WifiHandler h_wifi;

int mode_index;
int request;
String configData;

WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");
    // Inititalize Button and EEPROM Reading
    initEEPROM();

    // Read Address values for mode and request type
    mode_index = readEEPROMValue(MODE_ADDRESS);
    request = readEEPROMValue(REQUEST_ADDRESS);

    // Inititalize handlers
    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);
    h_bluetooth = BluetoothHandler();

    bool connect_test, send_test = false;

    // Have device respond based on what kind of request was sent
    switch (request)
    {
    case SLEEP_REQUEST:
        // Switch mode index back to regular operation
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

                // Have LED blink if email sent successfully
                if (send_test)
                {
                    wakeUpBlink();
                }
            }
            h_wifi.closeConnection();
        }

        // Keep operation in bluetooth server mode
        mode_index = BLE_MODE;

        break;
    case CONFIG_REQUEST:
        Serial.println("Configuration Requested");

        // Read Configuration data from EEPROM
        configData = readConfigData();
        if (configData.length() > 0)
        {
            // Get module and location from config data
            String module = getMessageString(configData, ',', 0);
            String location = getMessageString(configData, ',', 1);
            Serial.printf("Module: %s\nLocation: %s\n", module, location);
        }
        // Keep operation in bluetooth server mode
        mode_index = BLE_MODE;

        break;
    default:
        Serial.println("No Requests");
    }

    // After request was handled save to EEPROM we no longer have a request
    writeToEEPROM(REQUEST_ADDRESS, NO_REQUEST);

    // Write to EEPROM current mode of operation
    // If device was not in Regular mode, switch to regular mode, otherwise switch to BLE mode 
    writeToEEPROM(MODE_ADDRESS, mode_index != REGULAR_MODE ? REGULAR_MODE : BLE_MODE);
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

                // Have LED blink if email sent successfully
                if (send_success)
                {
                    wakeUpBlink();
                }
            }
            h_wifi.closeConnection();
        }
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Timer Wakeup");
        break;
    default:
        // Boot up or reset triggered
        
        // If device in BLE mode, initialize bluetooth server
        if (mode_index == BLE_MODE)
        {
            Serial.println("Bluetooth Server Mode");
            h_bluetooth.initServer();

            // Busy wait
            while (mode_index)
            {
            }
        }
        // otherwise in regular mode and end function
        else
        {
            Serial.println("Regular Mode");
            break;
        }
    }
}