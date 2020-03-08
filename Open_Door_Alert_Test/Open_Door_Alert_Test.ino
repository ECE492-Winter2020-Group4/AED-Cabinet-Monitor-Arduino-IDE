#include "WifiHandler.hpp"
#include "EmailHandler.hpp"
#include "defs.hpp"

WifiHandler h_wifi = WifiHandler();
EmailHandler h_email = EmailHandler(&h_wifi);
int door_status_prev = DOOR_CLOSE;

void setup()
{
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);

  pinMode(GPIO_DETECTOR, INPUT_PULLUP);

  // Connect to wifi
  h_wifi.initConnection();
}

void loop()
{
  int door_status_curr = digitalRead(gpio_num_t(GPIO_DETECTOR));
  digitalWrite(gpio_num_t(GPIO_LED), door_status_curr); // Safety Check
  if (door_status_curr == DOOR_OPEN)
  {
    Serial.println("Door is open!");
    // Send gmail if door just opened from being close
    if (door_status_prev == DOOR_CLOSE) {
      h_email.sendTestMsg();
    }
  }
  else
  {
    Serial.println("Door is close.");
  }
  door_status_prev = door_status_curr;
  delay(2000);
}
