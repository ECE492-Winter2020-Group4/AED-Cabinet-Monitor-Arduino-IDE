/*
main.cpp

This is the file that is needed by the arduino compiler. 
This must be located in the root directory of the project 
inorder for the compiler to pull all other files in.

*/
#include <Arduino.h>
#include <esp32-hal.h>
#include <esp_sleep.h>
#include "include/GpioHandler.hpp"
#include "include/WakeupHandler.hpp"
#include "include/WifiHandler.hpp"

#include "include/defs.hpp"

WifiHandler h_wifi;
GpioHandler h_gpio;
WakeupHandler h_wakeup; 

int enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;

void setup()
{
    Serial.begin(115200);
    h_wifi = WifiHandler();
    h_gpio = GpioHandler();
    h_wakeup = WakeupHandler();
}

void loop()
{
    // Will not actually loop due to how deep-sleep works.
    // Treat this as main()

    h_wakeup.handle();
    while(enterSleepInSec-- > 0)
    {
        vTaskDelay(1000);
        Serial.printf("Entering Sleep in %d\n", enterSleepInSec);
    }
    Serial.printf("Entering Deep Sleep for %d\n", DEEP_SLEEP_FOR_X_SECONDS);
    esp_deep_sleep(DEEP_SLEEP_FOR_X_SECONDS * 1000000);
}