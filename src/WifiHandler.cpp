#include "../include/WifiHandler.hpp"

WifiHandler::WifiHandler()
{
    Serial.println("Creating WifiHandler");
}

/**
 * Initiate wifi connection
 */
void WifiHandler::initConnection()
{
    // Initialize
    connection_state = 0;
    local_ip_address = "";

    // Connect to wifi
    connection_state = wifiConnect();
    if (!connection_state) // if not connected to WIFI
    {
        reconnect(); // trying to connect again for # attempts
    }

    // Connection successful
    if (connection_state)
    {
        Serial.print("Connected to ");
        Serial.println(WIFI_SSID);
    }
    // Connection failed
    else
    {
        Serial.print("Failed to connect to ");
        Serial.println(WIFI_SSID);
    }
}

/**
 * Close wifi connection and reset the connection status
 */
void WifiHandler::closeConnection()
{
    WiFi.disconnect(true);
    connection_state = 0;
}

/**
 * Attemp to connect to wifi and return the connection status
 */
uint8_t WifiHandler::wifiConnect()
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    Serial.println(WIFI_SSID);

    WiFi.disconnect(true);

    // connect to WPA2 enterprise wifi
    if (WIFI_TYPE == ENTERPRISE_WIFI)
    {
        WiFi.mode(WIFI_STA); //init wifi mode
        esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_ANONYMOUS_IDENTITY, strlen(EAP_ANONYMOUS_IDENTITY));
        esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
        esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
        esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
        esp_wifi_sta_wpa2_ent_enable(&config);                 //set config settings to enable function

        Serial.print("Begin WiFi");
        WiFi.begin(WIFI_SSID); //connect to wifi
    }
    // connect to non-enterprise wifi
    else
    {
        Serial.print("Begin WiFi");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //connect to wifi
    }

    // wait for wifi connection status
    int counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
        counter++;
        // wifi connection time out: connection failed
        if (counter >= connect_wait_time)
        {
            //ESP.restart();
            Serial.println("");
            return false;
        }
    }

    local_ip_address = WiFi.localIP().toString();
    Serial.println("");
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(local_ip_address);
    return true;
}

/**
 * Attempt to reconnect to wifi
 */
void WifiHandler::reconnect()
{
    uint8_t reconnect_attempt = 0;
    uint32_t ts = millis();
    while (!connection_state && reconnect_attempt < max_reconnect_attempts)
    {
        delay(500); // wait for 0.5 sec
        if (millis() > (ts + connect_wait_time * 1000) && !connection_state)
        {
            Serial.print("Connection failed, reconnect attempt ");
            Serial.println(reconnect_attempt + 1);

            connection_state = wifiConnect();
            ts = millis();
            reconnect_attempt += 1;
        }
    }
}

bool WifiHandler::getConnectionStatus()
{
    return (bool)connection_state;
}

String WifiHandler::getLocalIPAdress()
{
    return local_ip_address;
}