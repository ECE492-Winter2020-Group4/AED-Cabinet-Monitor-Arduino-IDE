#pragma once

#include <Arduino.h>
#include "WifiHandler.hpp"
#include "Gsender_32.h"
#include "defs.hpp"

class EmailHandler
{
public:
    EmailHandler(); // not used
    EmailHandler(WifiHandler *wifiHandler);
    static Gsender *gsender;
    void sendTestMsg();
    void sendOpenDoorAlert();
    void sendLowBatteryAlert();
    bool getEmailStatus();

private:
    WifiHandler *h_wifi;
    uint8_t sent_state = 0; // sent email or not
    void sendEmail(const String &subject, const String &content);
};