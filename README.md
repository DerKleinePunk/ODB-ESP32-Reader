# Read ODB with an ESP32

copy config_sample.hpp to config.hpp and config your values

## Serial Commands

Send Command to ESP via Serial Console

```console
s -> Scan BT for devices
c<index> connect Serial BT Device c2 -> Connect 2 Device from last scan.
r -> read rpm and kph (km/h)
mqtt -> search for mqtt
light -> Switch State from LED_BUILTIN
oi -> read supportet PID and Motorstate (MIL)
```

## Troubleshooting
+ Due to challenges connecting this to ELM327 wifi consistently I recommend bluetooth or a wired connection. For more details see [ELMduino Wifi Issue](PowerBroker2/ELMduino#52).

## Links / Helping Pages

+ [upload-code-esp32-cam-mb-usb](https://randomnerdtutorials.com/upload-code-esp32-cam-mb-usb/)
+ [obdon-board-diagnosticsdiagnostic](https://iamleon99.blogspot.com/2023/04/obdon-board-diagnosticsdiagnostic.html)
+ [ELMduino](https://github.com/PowerBroker2/ELMduino)
+ [android](https://www.kai-morich.de/android/)
+ [usb-serial-for-android-mik3y](https://github.com/kai-morich/usb-serial-for-android-mik3y)
+ [ibeacon-itag-praesenz-schalter](https://esp32-server.de/ibeacon-itag-praesenz-schalter/)
+ [stecker-belegungen](https://www.obd-2.de/stecker-belegungen.html)
+ [350z-drift-elmduino](https://github.com/michealengland/350z-drift-elmduino)
+ [esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication](https://circuitdigest.com/microcontroller-projects/esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication)
+ [OBD-II_PIDs](https://en.wikipedia.org/wiki/OBD-II_PIDs)
+ [programmierer-tips](https://www.obd-2.de/programmierer-tips.html)
+ [HA mqtt-discovery](https://www.home-assistant.io/integrations/mqtt#mqtt-discovery)
+ [avahi mosquitto](https://github.com/arendst/Tasmota/discussions/11403)
+ [mdns for esp](https://www.megunolink.com/articles/wireless/find-esp32-esp8266-ip-address-with-mdns/)
+ [arduino-home-assistant](https://github.com/dawidchyrzynski/arduino-home-assistant)
+ [arduino-mqtt](https://github.com/256dpi/arduino-mqtt)
+ [DnWiFiDoorLock](https://github.com/MacDada/DnWiFiDoorLock)
+ [platformio-version-increment](https://community.platformio.org/t/platformio-version-increment/13945/21)
+ [jake-is-ESD-protected](https://github.com/jake-is-ESD-protected)

## Remindings
 pio lib install "256dpi/MQTT"
 pio lib install "256dpi/arduino-mqtt"
 pio lib install "PubSubClient"
 pio pkg install -l "256dpi/arduino-mqtt" -d. -pespressif32 