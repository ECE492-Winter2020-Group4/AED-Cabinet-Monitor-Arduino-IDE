#include <Arduino.h>

#include "../include/defs.hpp"
#include "../include/EmailHandler.hpp"
#include "../include/GpioHandler.hpp"
#include "../include/WakeupHandler.hpp"

EmailHandler h_email;
WifiHandler h_wifi;

int mode_index;
int request;
String configData;

WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");

    // Inititalize handlers
    h_wifi = WifiHandler();
    h_email = EmailHandler(&h_wifi);

    bool connect_success = false;
    bool send_success = false;
}

void WakeupHandler::handle()
{
    bool connect_success = false;
    bool send_success = false;

    switch (esp_sleep_get_wakeup_cause())
    {
    // Detected door opening
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("ISR0 Wakeup");
        // connect to wifi and send email alert
        // repeat again if connected to wifi but failed to send alert
        for (int i = 0; i == 0 || (i < 2 && connect_success && !send_success); i++)
        {
            Serial.print("Round ");
            Serial.println(i + 1);

            // connect to wifi
            h_wifi.initConnection();
            connect_success = h_wifi.getConnectionStatus();
            // send open door alert email
            if (connect_success)
            {
                Serial.println("Success");
                blinkXTimes(2);
                delay(500); // wait for 0.5 sec before sending email
                h_email.sendOpenDoorAlert();
                send_success = h_email.getEmailStatus();

                // Have LED blink if email sent successfully
                if (send_success)
                {
                    blinkXTimes(3);
                }
            }
            Serial.println("Success");
            h_wifi.closeConnection();
        }
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Timer Wakeup");
        break;
    default:
        // Boot up or reset triggered

        Serial.println("Regular Mode");
    }
}