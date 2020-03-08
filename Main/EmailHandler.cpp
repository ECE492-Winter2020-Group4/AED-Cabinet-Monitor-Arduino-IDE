#include "EmailHandler.hpp"

Gsender *EmailHandler::gsender = Gsender::Instance();
String receiver_address[] = {RECEIVER_EMAIL_ADDRESS}; //{"jiayun@ualberta.ca", "kebbi@ualberta.ca", "dn2@ualberta.ca", "jicheng@ualberta.ca"};

EmailHandler::EmailHandler(WifiHandler *wifiHandler)
{
  Serial.println("Creating EmailHandler");
  h_wifi = wifiHandler;
}

void EmailHandler::sendTestMsg()
{
  Serial.println("Sending test email...");
  /*Serial.print("status:");
    Serial.println(h_wifi->getConnectionStatus());
    Serial.println("ip:"+h_wifi->getLocalIPAdress());*/
  String subject = "ESP32 Test from " + String(WIFI_SSID);
  String content = "Sent via local ip: " + h_wifi->getLocalIPAdress();
  sendEmail(subject, content);
}

void EmailHandler::sendOpenDoorAlert()
{
  Serial.println("Sending alert when AED door is open...");
  Serial.print("status:");
  Serial.println(h_wifi->getConnectionStatus());
  Serial.println("ip:" + h_wifi->getLocalIPAdress());
}

void EmailHandler::sendLowBatteryAlert()
{
  Serial.println("Sending alert when battery power is low...");
  Serial.print("status:");
  Serial.println(h_wifi->getConnectionStatus());
  Serial.println("ip:" + h_wifi->getLocalIPAdress());
}

void EmailHandler::sendEmail(const String &subject, const String &content)
{
  if (!h_wifi->getConnectionStatus())
  {
    Serial.println("No wifi connection. Cannot send email!");
    return;
  }

  int numOfReceivers = sizeof(receiver_address) / sizeof(receiver_address[0]);
  if (gsender->Subject(subject)->Send(numOfReceivers, receiver_address, content))
  {
    Serial.println("Message send.");
  }
  else
  {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }
}
