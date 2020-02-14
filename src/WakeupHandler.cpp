#include <Arduino.h>
#include "../include/WakeupHandler.hpp"
#include "../include/defs.hpp"
#include  "../include/EmailHandler.hpp"

EmailHandler h_email;
WakeupHandler::WakeupHandler()
{
    Serial.println("Creating WakeupHandler");
    h_email = EmailHandler();
}

void WakeupHandler::handle()
{
    switch(esp_sleep_get_wakeup_cause())
    {
        case ESP_SLEEP_WAKEUP_EXT0 : 
            Serial.println("ISR0 Wakeup"); 
            h_email.sendUnexpectedOpening();
            break;
        case ESP_SLEEP_WAKEUP_TIMER : 
            Serial.println("Timeer Wakeup"); 
            // TODO : LOG
            break;
        default : 
            // Only at boot up, Do Nothing
            Serial.printf("Other Wakeup: %d\n", esp_sleep_get_wakeup_cause());
            break;
    }
}