#include <Arduino.h>
#include "../include/EmailHandler.hpp"
#include "../include/defs.hpp"


EmailHandler::EmailHandler()
{
    Serial.println("Creating EmailHandler");
}

bool EmailHandler::sendUnexpectedOpening()
{
    // TODO
    Serial.println("Sending Unexpected Opening Email");
    return true;
}
