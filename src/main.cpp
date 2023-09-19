#include <Arduino.h>

// BT: Bluetooth availabilty check
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)                           
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
// BT: Serial Bluetooth availabilty check
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#include "config.hpp"
#include "wlan_connect.hpp"
#include "bluetooth_connect.hpp"
#include "command_reader.hpp"

#ifndef LED_BUILTIN
  #define LED_BUILTIN 4
#endif

WlanConnect wlan = WlanConnect(ssid, password);
BluetoothConnect* bleConnect;
CommandReader commandReader;

void setup() {
  
  Serial.begin(115200);
  //Todo IF def DEBUG
  Serial.setDebugOutput(true);

  pinMode(LED_BUILTIN, OUTPUT);//Specify that LED pin is output

  wlan.Init();

  bleConnect = new BluetoothConnect();
  commandReader.begin(Serial);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(F("Loop"));

  if(wlan.Check()) {
    
  }

  const auto command = commandReader.GetCommand();
  if(command.length() > 0) {
    Serial.println("Command: "  + command);
    if(command == "s") {
      bleConnect->Scan();
      Serial.println(F("BT Scan finish"));
    }
    
  }

  //delay(5000);
}