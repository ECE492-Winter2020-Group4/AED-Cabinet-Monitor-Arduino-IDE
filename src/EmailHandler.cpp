#include "../include/EmailHandler.hpp"

Gsender *EmailHandler::gsender = Gsender::Instance();
// receiver_address could be a list of receivers: e.g. {"a@ualberta.ca", "b@ualberta.ca", "c@ualberta.ca", "d@ualberta.ca"};
String receiver_address[] = {RECEIVER_EMAIL_ADDRESS};
const String automated_msg = "This is an automated message.<br>Please do not reply.";

/**
 * Not used!
 */
EmailHandler::EmailHandler()
{
}

/**
 * Initialization
 */
EmailHandler::EmailHandler(WifiHandler *wifiHandler)
{
    Serial.println("Creating EmailHandler");
    h_wifi = wifiHandler;
}

/**
 * Send an email for testing
 */
void EmailHandler::sendTestMsg()
{
    Serial.println("Sending test email...");


    String subject = "ESP32 Test from " + String(WIFI_SSID);
    String content = "Sent via local ip: " + h_wifi->getLocalIPAdress() + "<br>" + "Module: " + MODULE + "<br>Location: " + LOCATION + "<br><br>";
    sendEmail(subject, content);

    delay(send_wait_time * 1000);
    if (!sent_state)              // if not successful
        resend(subject, content); // trying to send again for # attempts
}

/**
 * Send an alert email about AED door being open
 */
void EmailHandler::sendOpenDoorAlert()
{
    Serial.println("Sending alert when connected to Email Server.");


    String subject = "AED cabinet - open door alert";
    String content = "Module: " + module + "<br>Location: " + LOCATION + "<br><br>" + automated_msg;
    sendEmail(subject, content);

    delay(send_wait_time * 1000);
    if (!sent_state)              // if not successful
        resend(subject, content); // trying to send again for # attempts
}

/**
 * Attempt to send an email
 */
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
        Serial.print("Sending message from ");
        Serial.print(SENDER_GMAIL_ADDRESS);
        Serial.print(" to ");
        Serial.println(RECEIVER_EMAIL_ADDRESS);
        sent_state = 1;
    }
    else
    {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
        sent_state = 0;
    }
}

/**
 * Attempt to resend email until successful or max # attempts is reached
 */
void EmailHandler::resend(const String &subject, const String &content)
{
    uint8_t resend_attempt = 0;
    uint32_t ts = millis();
    while (h_wifi->getConnectionStatus() && !sent_state && resend_attempt < max_resend_attempts)
    {
        Serial.println("");
        Serial.print("Resend attempt ");
        Serial.println(resend_attempt + 1);

        sendEmail(subject, content);
        resend_attempt += 1;

        delay(send_wait_time * 1000);
    }
}


/**
 * Attempt to resend email until successful or max # attempts is reached
 */
bool EmailHandler::getEmailStatus()
{
    return (bool)sent_state;
}