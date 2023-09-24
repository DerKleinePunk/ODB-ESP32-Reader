#include <Arduino.h>

// BT: Bluetooth availabilty check
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run make menuconfig to and enable it
#endif
// BT: Serial Bluetooth availabilty check
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#include "elm327_connect.hpp"
#include "command_reader.hpp"
#include "config.hpp"
#include "wlan_connect.hpp"
#include "mqtt_connect.hpp"

#ifndef LED_BUILTIN
#define LED_BUILTIN 4
#endif

WlanConnect wlan = WlanConnect(ssid, password);
Elm327Connect* elm327Connect;
CommandReader commandReader;
MqttConnect mqttConnect;

void odbStateChanged(obd_pid_states pid, MotorState state) {
    Serial.println(F("MotorState"));
    Serial.printf("RPM: %f", state.rpm);
}

void setup()
{
    Serial.begin(115200);
    // Todo IF def DEBUG
    Serial.setDebugOutput(true);

    pinMode(LED_BUILTIN, OUTPUT); // Specify that LED pin is output

    wlan.Init();

    elm327Connect = new Elm327Connect();
    commandReader.begin(Serial);
    elm327Connect->ValueChangedCallback(odbStateChanged);
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Serial.println(F("Loop"));

    if(wlan.Check()) {
    }

    const auto commands = commandReader.GetCommand();
    if(!commands.empty()) {
        for(const auto command : commands) {
            Serial.println("Command: " + command);
            if(command == "s") {
                Serial.println(F("BT Scan running"));
                elm327Connect->Scan();
                Serial.println(F("BT Scan finish"));
            }
            if(command[0] == 'c') {
                byte index = atoi(command.substring(1).c_str());
                elm327Connect->ConnectSerial(index);
            }
            if(command[0] == 'd') {
                elm327Connect->Disconnect();
            }
            if(command[0] == 'r') {
                elm327Connect->rpm();
                elm327Connect->kph();
            }
            if(command == "mqtt") {
                mqttConnect.Search();
            }
        }
    }
    elm327Connect->loop();
}