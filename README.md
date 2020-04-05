Please see Application Notes for full usage: https://docs.google.com/document/d/1z3vVcc0KK2gyxqZ8YIz-DgapLzbS-YSmvJv0rFvbp5M/edit

## How to run the program in VS Code:
Please ensure the 'Arduino' module is installed for VS Code. 
1. Hit 'f1', and selected 'board config', and choose 'esp32 sparkfun thing'
2. Hit 'f1', and select 'select serial port', and choose the correct option
3. Hit 'f1', and select 'upload', and hopefully it should work (* for Mac user, please press and hold the boot button when uploading)

## Please modify the following parameters at include/defs.hpp before running the program.
For UWS:
1. WIFI_SSID: UWS
2. EAP_IDENTITY: [ualberta ccid]
3. EAP_PASSWORD: [ualberta password]
4. WIFI_TYPE: ENTERPRISE_WIFI
5. SENDER_GMAIL_ADDRESS: uaececapstone@gmail.com [sender gmail account]
6. SENDER_GMAIL_PASSWORD: [sender gmail account password]
7. RECEIVER_EMAIL_ADDRESS: [receiver email account]

For home wifi:
1. WIFI_SSID: [home wifi name]
2. WIFI_PASSWORD: [home wifi password]
3. WIFI_TYPE: HOME_WIFI
4. SENDER_GMAIL_ADDRESS: uaececapstone@gmail.com [sender gmail account]
5. SENDER_GMAIL_PASSWORD: [sender gmail account password]
6. RECEIVER_EMAIL_ADDRESS: [receiver email account]
