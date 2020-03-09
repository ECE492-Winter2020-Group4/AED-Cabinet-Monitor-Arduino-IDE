#include "../include/EmailHandler.hpp"

Gsender *EmailHandler::gsender = Gsender::Instance();
String receiver_address[] = {RECEIVER_EMAIL_ADDRESS};
//{"jiayun@ualberta.ca", "kebbi@ualberta.ca", "dn2@ualberta.ca", "jicheng@ualberta.ca"};
String automated_msg = "This is an automated message. Please do not reply.";

// not used
EmailHandler::EmailHandler()
{
}

EmailHandler::EmailHandler(WifiHandler *wifiHandler)
{
    Serial.println("Creating EmailHandler");
    h_wifi = wifiHandler;
}

void EmailHandler::sendTestMsg()
{
    Serial.println("Sending test email...");
    /*Serial.printf("status: %d\n", h_wifi->getConnectionStatus());
    Serial.println("ip:" + h_wifi->getLocalIPAdress());*/
    String subject = "ESP32 Test from " + String(WIFI_SSID);
    String content = "Sent via local ip: " + h_wifi->getLocalIPAdress();
    sendEmail(subject, content);
}

void EmailHandler::sendOpenDoorAlert()
{
    Serial.println("Sending alert when AED door is open...");
    String subject = "AED cabinet - open door alert";
    String content = "Module: " + String(MODULE) + "<br>Location: " + String(LOCATION) + "<br><br>" + automated_msg;
    sendEmail(subject, content);
}

void EmailHandler::sendLowBatteryAlert()
{
    Serial.println("Sending alert when battery power is low...");
    String subject = "AED cabinet - low battery power alert";
    String content = "Module: " + String(MODULE) + "<br>Location: " + String(LOCATION) + "<br><br>" + automated_msg;
    sendEmail(subject, content);
}

void EmailHandler::sendEmail(const String &subject, const String &content)
{
    if (!h_wifi->getConnectionStatus())
    {
        Serial.println("No wifi connection. Cannot send email!");
        sent_state = 0;
    }

    int numOfReceivers = sizeof(receiver_address) / sizeof(receiver_address[0]);
    if (gsender->Subject(subject)->Send(numOfReceivers, receiver_address, content))
    {
        Serial.println("Message send.");
        sent_state = 1;
    }
    else
    {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
        sent_state = 0;
    }
}

bool EmailHandler::getEmailStatus()
{
    return (bool)sent_state;
}