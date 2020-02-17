/* Arduino core for ESP32 WiFi chip SMTP Gmail sender
   by jubaid hossain
   4.9.2018
   you can copy and paste full code
*/
#include <WiFi.h>
#include "Gsender_32.h"
#include <WiFiClientSecure.h>
#include "esp_wpa2.h"

#pragma region Globals
//-------- Customise these values -----------
const char* ssid = "UWS";
#define EAP_ANONYMOUS_IDENTITY "anonymous"
#define EAP_IDENTITY "******"                 // ualberta ccid
#define EAP_PASSWORD "******"                 // ualberta password
//-------- END Customise these values -----------
uint8_t connection_state = 0;                 // Connected to WIFI or not
uint16_t reconnect_interval = 10000;          // If not connected wait time to try again
#pragma endregion Globals

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD    115200

String address[] = {"jiayun@ualberta.ca", "kebbi@ualberta.ca", "dn2@ualberta.ca", "jicheng@ualberta.ca"};
//String address[] = {"jiayun@ualberta.ca"};
String ip_address = "";

uint8_t WiFiConnect()
{
  static uint16_t attempt = 0;
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  // WPA2 enterprise magic starts here
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  // WPA2 enterprise magic ends here

  Serial.print("Begin WiFi");
  WiFi.begin(ssid); //connect to wifi

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
      return false;
    }
  }

  ip_address = WiFi.localIP().toString();
  Serial.println("");
  Serial.println("Connection: ESTABLISHED");
  Serial.print("Got IP address: ");
  Serial.println(ip_address);
  return true;
}

void Awaits()
{
  uint32_t ts = millis();
  while (!connection_state)
  {
    delay(200);
    if (millis() > (ts + reconnect_interval) && !connection_state)
    {
      connection_state = WiFiConnect();
      ts = millis();
    }
  }
}

void setup()
{
  //Initialize serial and wait for port to open:
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(100);
  connection_state = WiFiConnect();
  if (!connection_state) // if not connected to WIFI
  {
    Awaits(); // constantly trying to connect
  }
  Serial.print("Connected to ");
  Serial.println(ssid);

  Gsender *gsender = Gsender::Instance(); // Getting pointer to class instance
  String subject = "ESP32 Test from UWS :)";
  String content = "ESP32 Test Email sent from university wifi via local ip: " + ip_address;
  int numOfReceivers = sizeof(address) / sizeof(address[0]);
  if (gsender->Subject(subject)->Send(numOfReceivers, address, content))
  {
    Serial.println("Message send.");
  }
  else
  {
    Serial.print("Error sending message: ");
    Serial.println(gsender->getError());
  }
}

void loop() {}
