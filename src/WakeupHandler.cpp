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
    Serial.print("mode_index : ");
    Serial.println(mode_index);

    EEPROM.write(MODE_ADDRESS, mode_index != 0 ? 0 : 1);
    EEPROM.commit();

    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);
    h_bluetooth = BluetoothHandler();
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
        Serial.printf("Button Reset Wakeup: %d\n", esp_sleep_get_wakeup_cause());
        if (mode_index == 1)
        {
            Serial.println("Bluetooth Server Mode");
            while (mode_index)
            {
                h_bluetooth.initServer();
            }
        }
        else
        {
            Serial.println("Sleep Mode");
            break;
        }
    }
}