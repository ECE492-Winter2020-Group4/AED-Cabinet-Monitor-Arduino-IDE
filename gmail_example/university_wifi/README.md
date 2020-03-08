This ESP32 Gmail Example Project connects to UWS (enterprise) wifi and sends out a test email.

Gsender is an adaptation of the ESP8266 Gmail sender program. It allows for ESP32 to send emails to one recipient or multiple. The code can be downloaded from https://github.com/AIWIndustries/ESP32_Gmail_Sender. Before using you will need to base64encode your email information you will use to log into gmail with as per this ESP8266 instructional, https://www.instructables.com/id/ESP8266-GMail-Sender/. Then edit the .h file (the Gsender_32.h file in this case). The sketch is where you add your wifi credentials and potential email recipients.

# Required modifications:
1. "ESP32_Gmail_Sender.ino": EAP_IDENTITY (ualberta ccid) and EAP_PASSWORD (ualberta password)
2. "Gsender_32": base64_encoded email password

# Required Arduino libraries:
1. WiFi
2. WiFiClientSecure