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

#include "esp32-hal-cpu.h"
#include "esp_pm.h"
#include "include/defs.hpp"
#include <driver/gpio.h>
#include <driver/rtc_io.h>
#include <esp_wifi.h>

GpioHandler h_gpio;
WakeupHandler h_wakeup;

int enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;

gpio_num_t disabledGPIO[] = {
    GPIO_NUM_0,
    GPIO_NUM_1,
    GPIO_NUM_2,
    GPIO_NUM_3,
    GPIO_NUM_4,
    GPIO_NUM_5,
    GPIO_NUM_6,
    GPIO_NUM_7,
    GPIO_NUM_8,
    GPIO_NUM_9,
    GPIO_NUM_10,
    GPIO_NUM_11,
    GPIO_NUM_12,
    GPIO_NUM_13,
    GPIO_NUM_14,
    // GPIO_NUM_15, // Magnetic Detector
    GPIO_NUM_16,
    GPIO_NUM_17,
    GPIO_NUM_18,
    GPIO_NUM_19,
    GPIO_NUM_21,
    GPIO_NUM_22,
    GPIO_NUM_23,
    GPIO_NUM_25,
    GPIO_NUM_26,
    GPIO_NUM_27,
    GPIO_NUM_32,
    GPIO_NUM_33,
    GPIO_NUM_34,
    GPIO_NUM_35,
    GPIO_NUM_36,
    GPIO_NUM_37,
    GPIO_NUM_38,
    GPIO_NUM_39};

void powerOptimization()
{
    setCpuFrequencyMhz(80); // Minimum possible for Wifi, and BT

    // Disable RTC memory when in deep sleep
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);

    // Disable XTAL-Oscelator when in Deep sleep Mode
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_OFF);

    // Configure default power domain to MAX power saving
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);

    // Disable ROM logging in deep sleep mode
    esp_deep_sleep_disable_rom_logging();

    // Disable GPIO Memory when in deep sleep mode
    rtc_gpio_force_hold_dis_all();

    // Apply to all unused GPIO pins
    for (gpio_num_t i : disabledGPIO)
    {
        // Disable Pullup/Pulldown resistors when in deep sleep mode
        rtc_gpio_isolate(i);
        gpio_pulldown_dis(i);
        gpio_pullup_dis(i);
    }
}

void setup()
{
    Serial.begin(115200);   // Default Baud rate to connect the terminal to.
    h_gpio = GpioHandler();
    h_wakeup = WakeupHandler();
    powerOptimization();
}

void loop()
{
    // Will not actually loop due to how deep-sleep works.
    // This will execute after setup() is called. 
    // Will execute once, then enter deep sleep at the end
    // Treat this as main()

    // Handle the wakeup cause accordingly
    h_wakeup.handle();

    // Wait until STAY_AWAKE_FOR_X_SECONDS seconds has elapsed, then go to sleep.
    // If door is opened before the timer expires, reset it to STAY_AWAKE_FOR_X_SECONDS, and start counting again
    while (enterSleepInSec-- > 0)
    {
        if (digitalRead(gpio_num_t(GPIO_DETECTOR)) == 1)
        {
            // Door is open
            enterSleepInSec = STAY_AWAKE_FOR_X_SECONDS;
            Serial.printf("Staying awake\n");
        }
        else
        {
            // Door is closed
            Serial.printf("Entering Sleep in %d sec\n", enterSleepInSec);
        }
        // LED for visual cue
        digitalWrite(gpio_num_t(GPIO_LED), digitalRead(gpio_num_t(GPIO_DETECTOR))); // Safety Check
        vTaskDelay(1000);
    }
    Serial.printf("Entering Deep Sleep for %d sec\n", DEEP_SLEEP_FOR_X_SECONDS);

    // Deep Sleep
    esp_deep_sleep(DEEP_SLEEP_FOR_X_SECONDS * 1000000); // Arg is in microseconds
}