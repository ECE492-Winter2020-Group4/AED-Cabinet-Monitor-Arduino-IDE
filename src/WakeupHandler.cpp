#include <Arduino.h>
#include "../include/WakeupHandler.hpp"
#include "../include/defs.hpp"
#include "../include/EmailHandler.hpp"

WifiHandler h_wifi;
EmailHandler h_email;

WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");
    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);
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
        // Only at boot up, Do Nothing
        Serial.printf("Other Wakeup: %d\n", esp_sleep_get_wakeup_cause());
        break;
    }
}