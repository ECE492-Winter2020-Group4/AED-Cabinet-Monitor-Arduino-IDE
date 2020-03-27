#pragma once

#include <Arduino.h>
#include "EEPROM.h"
#include "../include/defs.hpp"

void initEEPROM();
int readEEPROMValue(int address);
String getMessageString(String data, char separator, int index);
String readConfigData();
void writeConfig(String data);
void writeToEEPROM(int address, int value);
