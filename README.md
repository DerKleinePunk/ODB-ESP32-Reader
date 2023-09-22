# Read ODB with an ESP32

copy config_sample.hpp to config.hpp and config your values

## Serial Commands

Send Command to ESP via Serial Console

```console
s -> Scan BT for devices
c<index> connect Serial BT Device c2 -> Connect 2 Device from last scan.
r -> read rpm and kph (km/h)
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