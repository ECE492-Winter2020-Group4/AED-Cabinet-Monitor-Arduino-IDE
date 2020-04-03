#include "../include/EEPROMHelper.hpp"

void initEEPROM(){
    if (!EEPROM.begin(EEPROM_SIZE))
    {
        delay(1000);
    }
}

// Read EEPROM value from given address
int readEEPROMValue(int address){
    return EEPROM.read(address);
}

// Returns either module or location by parsing config data based on index
String getMessageString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// Read configuration data stored in config address from EEPROM
String readConfigData()
{
    char data[100];
    int len = 0;
    unsigned char k;
    k = EEPROM.read(CONFIG_ADDRESS);
    while (k != '\0' && len < 500)
    {
        k = EEPROM.read(CONFIG_ADDRESS + len);
        data[len] = k;
        len++;
    }
    data[len] = '\0';
    return String(data);
}

// Write configuration data to EEPROM address
void writeConfig(String data)
{
    int data_size = data.length();
    for (int i = 0; i < data_size; i++)
    {
        EEPROM.write(CONFIG_ADDRESS + i, data[i]);
    }
    EEPROM.write(CONFIG_ADDRESS + data_size, '\0');
    EEPROM.commit();
}

void writeToEEPROM(int address, int value){
    EEPROM.write(address, value);
    EEPROM.commit();
}