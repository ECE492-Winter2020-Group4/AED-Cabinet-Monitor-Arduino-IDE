#include <Arduino.h>
#include "../include/GpioHandler.hpp"
#include "../include/defs.hpp"

bool ledStatus = false;
extern int enterSleepInSec;

GpioHandler::GpioHandler()
{
    Serial.println("Creating GpioHandler");
    pinMode(GPIO_LED, OUTPUT);
    pinMode(GPIO_DETECTOR, INPUT_PULLDOWN);
    attachInterrupt(GPIO_DETECTOR, isr, RISING); // On Button Release
    
    digitalWrite(GPIO_LED, HIGH);   
    gpio_pullup_en(gpio_num_t(GPIO_DETECTOR));
    gpio_pulldown_dis(gpio_num_t(GPIO_DETECTOR));
    esp_sleep_enable_ext0_wakeup(gpio_num_t(GPIO_DETECTOR), ESP_EXT1_WAKEUP_ANY_HIGH);
    
}

void GpioHandler::isr() {
    digitalWrite(GPIO_LED, ledStatus = !ledStatus);   
    enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;
}