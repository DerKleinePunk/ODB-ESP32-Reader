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
#ifdef MQTT_ENABLED
    #include "mqtt_connect.hpp"
    #include "HA/sensor.hpp"
#endif


#ifndef LED_BUILTIN
#define LED_BUILTIN 4
#endif

/* Returns a semi-unique id for the device. The id is based
*  on part of a MAC address or chip ID so it won't be 
*  globally unique. */
uint64_t GetDeviceId()
{
#if defined(ARDUINO_ARCH_ESP32)
  return ESP.getEfuseMac();
#else
  return ESP.getChipId();
#endif
}

/* Append a semi-unique id to the name template */
String MakeMine(const char *NameTemplate)
{
  String Result = String(NameTemplate) + String(GetDeviceId(), HEX);
  return Result;
}

WlanConnect wlan = WlanConnect(ssid, password);
Elm327Connect* elm327Connect;
CommandReader commandReader;

#ifdef MQTT_ENABLED
    MqttConnect mqttConnect;
#endif

void odbStateChanged(obd_pid_states pid, MotorState state) {
    Serial.print(F("MotorState"));
    switch (pid)
    {
    case obd_pid_states::COOLANT:
        Serial.printf("Engine Coolant Temp: %f", state.engineCoolantTemp);
        break;
    case obd_pid_states::OILTEMP:
        Serial.printf("OIL TEMP: %f", state.oilTemp);
        break;
    case obd_pid_states::INTAKE_AIR:
        Serial.printf("Intake Air Temp: %f", state.intakeAirTemp);
        break;
    case obd_pid_states::RPM:
        Serial.printf("RPM: %f", state.rpm);
        break;
    case obd_pid_states::KPH:
        Serial.printf("KPH: %f", state.kph);
        break;
    case obd_pid_states::SUPPORTEDPID_1_20:
        Serial.printf("Supported 1 - 20: %d", state.pid1_20);
        break;
    case obd_pid_states::MONITORSTATUS:
        Serial.printf("MotorState: %d", state.motorState);
        break;
    default:
        log_e("mission implementation for pid %d",pid);
    }
   Serial.println();
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
#ifdef MQTT_ENABLED
    mqttConnect.begin(wlan);
#endif
}

void loop()
{

    if(!wlan.Check()) {
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
                elm327Connect->coolantTemp();
                elm327Connect->oilTemp();
                elm327Connect->intakeAir();
                elm327Connect->rpm();
                elm327Connect->kph();
            }
            if(command[0] == 'oi') {
                elm327Connect->supportedPID();
                elm327Connect->monitorStatus();
            }
            if(command == "mqtt") {
#ifdef MQTT_ENABLED
                mqttConnect.Search();
#else
       Serial.println(F("No MQTT"));
#endif
            }
            if(command == "light") {
                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
            }
            if(command == "t1") {
#ifdef MQTT_ENABLED
                HA::sensor sensor("ODBII", String(GetDeviceId(), HEX).c_str());
                HA::device lightDevice("LIGHT", HA::device_types::SWITCH);
                
                sensor.addDevice(lightDevice);

                for(const auto entry : sensor.getConfigJson()) {
                    Serial.println(entry.topic.c_str());
                    Serial.println(entry.payload.c_str());

                    mqttConnect.publish(entry.topic, entry.payload);
                }
#endif
            }
        }
    }
    elm327Connect->loop();
}