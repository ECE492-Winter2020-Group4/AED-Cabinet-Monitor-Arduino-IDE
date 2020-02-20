#include <Arduino.h>
#include "../include/GpioHandler.hpp"
#include "../include/defs.hpp"

#include <driver/gpio.h>
#include <driver/rtc_io.h>

bool ledStatus = false;
extern int enterSleepInSec;

void wakeUpBlink();

GpioHandler::GpioHandler()
{
    Serial.println("Creating GpioHandler");
    wakeUpBlink();
    pinMode(GPIO_DETECTOR, INPUT_PULLUP);
    attachInterrupt(GPIO_DETECTOR, isr, CHANGE); 
    
    digitalWrite(GPIO_LED, HIGH);   
    esp_sleep_enable_ext0_wakeup(gpio_num_t(GPIO_DETECTOR), 1);
}

void GpioHandler::isr() {
    // Turn on/off blue LED
    digitalWrite(gpio_num_t(GPIO_LED), digitalRead(gpio_num_t(GPIO_DETECTOR)));
    enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;
}

void wakeUpBlink()
{
    for(int i = 0; i < 3 ; i++)
    {
        pinMode(GPIO_LED, INPUT);
        vTaskDelay(100);
        pinMode(GPIO_LED, OUTPUT);
        vTaskDelay(100);
    }
}