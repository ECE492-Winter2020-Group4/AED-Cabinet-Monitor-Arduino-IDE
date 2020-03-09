#pragma once

#ifndef G_SENDER
#define G_SENDER
#define GS_SERIAL_LOG_1 // Print to Serial only server responce
//#define GS_SERIAL_LOG_2       //  Print to Serial client commands and server responce
#include <WiFiClientSecure.h>
#include <base64.h>
#include "defs.hpp"

class Gsender
{
protected:
    Gsender();

private:
    const int SMTP_PORT = 465;
    const char *SMTP_SERVER = "smtp.gmail.com";
    //const char *EMAILBASE64_LOGIN = SENDER_GMAIL_BASE64_LOGIN;
    //const char *EMAILBASE64_PASSWORD = SENDER_GMAIL_BASE64_PASSWORD;
    String EMAILBASE64_LOGIN;
    String EMAILBASE64_PASSWORD;
    const char *FROM = SENDER_GMAIL_ADDRESS;
    const char *_error = nullptr;
    char *_subject = nullptr;
    String _serverResponce;
    static Gsender *_instance;
    bool AwaitSMTPResponse(WiFiClientSecure &client, const String &resp = "", uint16_t timeOut = 60000);
    void setupGmailCredential();
    String encodeBase64(String toEncode);

public:
    static Gsender *Instance();
    Gsender *Subject(const char *subject);
    Gsender *Subject(const String &subject);
    bool Send(const String &to, const String &message);
    bool Send(int num, const String to[], const String &message);
    String getLastResponce();
    const char *getError();
};
#endif // G_SENDER
