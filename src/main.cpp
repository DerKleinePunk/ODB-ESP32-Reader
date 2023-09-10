#include <Arduino.h>

#include "config.hpp"
#include "wlan_connect.hpp"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 4
#endif

WlanConnect wlan = WlanConnect(ssid, password);

void setup() {
  
  Serial.begin(115200);
  //Todo IF def DEBUG
  Serial.setDebugOutput(true);

  pinMode(LED_BUILTIN, OUTPUT);//Specify that LED pin is output

  wlan.Init();
}

void loop() {
  // put your main code here, to run repeatedly:

  wlan.Check();
  delay(5000);
}