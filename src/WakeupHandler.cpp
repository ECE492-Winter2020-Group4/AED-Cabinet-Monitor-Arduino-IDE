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
    switch (esp_sleep_get_wakeup_cause())
    {
    // Detected door opening
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("ISR0 Wakeup");
        // connect to wifi
        h_wifi.initConnection();
        // send open door alert email
        if (h_wifi.getConnectionStatus())
        {
            h_email.sendOpenDoorAlert();
            // resend email once if there is an error
            if (h_wifi.getConnectionStatus() && !h_email.getEmailStatus())
                h_email.sendOpenDoorAlert();
        }
        h_wifi.closeConnection();
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