#pragma once

#include <Arduino.h>
#include "WifiHandler.hpp"
#include "Gsender_32.h"
#include "defs.hpp"
#include "EEPROMHelper.hpp"

class EmailHandler
{
public:
    EmailHandler(); // not used
    EmailHandler(WifiHandler *wifiHandler);
    static Gsender *gsender;
    void sendTestMsg();
    void sendOpenDoorAlert();
    bool getEmailStatus();

private:
    WifiHandler *h_wifi;
    String module = MODULE;
    String location = LOCATION;
    uint8_t sent_state = 0;                                   // Sent email or not
    uint16_t send_wait_time = EMAIL_SEND_WAIT_TIME_SECONDS;   // Max wait time for email sent status
    uint16_t max_resend_attempts = MAX_EMAIL_RESEND_ATTEMPTS; // Max # of attempts for resending email
    void sendEmail(const String &subject, const String &content);
    void resend(const String &subject, const String &content);
    void getDeviceData();
};