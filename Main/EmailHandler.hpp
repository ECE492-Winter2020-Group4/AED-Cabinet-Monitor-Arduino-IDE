#pragma once

#include <Arduino.h>
#include "WifiHandler.hpp"
#include "Gsender_32.h"
#include "defs.hpp"

class EmailHandler
{
    public:
        EmailHandler(WifiHandler *wifiHandler);
        static Gsender *gsender;
        void sendTestMsg();
        void sendOpenDoorAlert();
        void sendLowBatteryAlert();
    private:
        WifiHandler *h_wifi;
        void sendEmail(const String &subject, const String &content);
};
