#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/rtc_io.h>

#include "../include/GpioHandler.hpp"
#include "../include/defs.hpp"

extern int enterSleepInSec; // Imported from Defs.hpp

GpioHandler::GpioHandler()
{
    Serial.println("Creating GpioHandler");
    wakeUpBlink();

    // Configure GPIO to support ISR
    pinMode(GPIO_DETECTOR, INPUT_PULLUP);
    attachInterrupt(GPIO_DETECTOR, isr, CHANGE); 
    
    digitalWrite(GPIO_LED, HIGH);   

    // Configure GPIO_DETECTOR to stay on when in sleep mode
    esp_sleep_enable_ext0_wakeup(gpio_num_t(GPIO_DETECTOR), 1);
}

void GpioHandler::isr() {
    // Turn on/off blue LED
    // On = Open
    // Off = Close
    digitalWrite(gpio_num_t(GPIO_LED), digitalRead(gpio_num_t(GPIO_DETECTOR)));

    // Reset the timer to the maximum in the event of detector going off before sleep mode is entered
    enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;
}

void wakeUpBlink()
{
    // Blink 3 times.
    for(int i = 0; i < 3 ; i++)
    {
        pinMode(GPIO_LED, INPUT);
        vTaskDelay(100);
        pinMode(GPIO_LED, OUTPUT);
        vTaskDelay(100);
    }
}