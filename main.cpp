/*
main.cpp

This is the file that is needed by the arduino compiler. 
This must be located in the root directory of the project 
inorder for the compiler to pull all other files in.

*/
#include <Arduino.h>
#include <esp32-hal.h>
#include <esp_sleep.h>

#include "include/defs.hpp"

bool ledStatus = false;
int enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;

void isr() {
    digitalWrite(GPIO_LED, ledStatus = !ledStatus);   
    enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;
}

void initializeGpio()
{
    pinMode(GPIO_LED, OUTPUT);
    pinMode(GPIO_DETECTOR, INPUT_PULLDOWN);
    attachInterrupt(GPIO_DETECTOR, isr, RISING); // On Button Release
    esp_sleep_enable_ext0_wakeup(gpio_num_t(GPIO_DETECTOR), ESP_EXT1_WAKEUP_ALL_LOW);

}

void handleWakeup()
{
  switch(esp_sleep_get_wakeup_cause())
  {
    case ESP_SLEEP_WAKEUP_EXT0 : 
        Serial.println("Wakeup caused by external signal using RTC_IO"); 
        break;
    case ESP_SLEEP_WAKEUP_EXT1 : 
        // Don't care
        Serial.println("Wakeup caused by external signal using RTC_CNTL"); 
        break;
    case ESP_SLEEP_WAKEUP_TIMER : 
        Serial.println("Wakeup caused by timer"); 
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : 
        // Don't care
        Serial.println("Wakeup caused by touchpad"); 
        break;
    case ESP_SLEEP_WAKEUP_ULP : 
        // Don't care
        Serial.println("Wakeup caused by ULP program"); 
        break;
    default : 
        // Only at boot up
        Serial.printf("Wakeup was not caused by deep sleep: %d\n",esp_sleep_get_wakeup_cause());
        break;
  }
}

void setup()
{
    Serial.begin(115200);
    initializeGpio();
    handleWakeup();
}

void loop()
{
    while(enterSleepInSec-- > 0)
    {
        vTaskDelay(1000);
        Serial.printf("Entering Sleep in %d\n", enterSleepInSec);
        
    }
    Serial.printf("Entering Deep Sleep for %d\n", DEEP_SLEEP_FOR_X_SECONDS);
    esp_deep_sleep(DEEP_SLEEP_FOR_X_SECONDS * 1000000);
}