#pragma once
#include <Arduino.h>
#include <BluetoothSerial.h>
// https://esp32-server.de/ibeacon-itag-praesenz-schalter/

#include <BLEAddress.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>

#include <vector>

#include "ELMduino.h"

enum struct obd_pid_states { NOTHING, COOLANT, OILTEMP, INTAKE_AIR, RPM, KPH };

struct btDevice {
    BTAddress address;
    String name;
    bool classic;
};

struct MotorState {
    float rpm;
    int32_t kph;
    float oilTemp;
};


class BluetoothConnect
{
    using ValueChangedEvent = void (*)(obd_pid_states, MotorState);

  private:
    BLEScan* _pBLEScan;
    u_int32_t _nScanTime;
    BluetoothSerial _SerialBT;
    bool _emlConnected;
    ELM327 _ELMduino;
    obd_pid_states _current_obd_pid;
    obd_pid_states _obd_pid_list[20];
    ValueChangedEvent _event;
    MotorState _motor_State;
    bool AddPidIfNotExits(obd_pid_states pid);
  public:
    BluetoothConnect();
    ~BluetoothConnect();

    std::vector<btDevice> m_pAddresses;

    void Scan();
    void ConnectSerial(byte index);
    void Disconnect();
    void ValueChangedCallback(ValueChangedEvent callback);
    void rpm();
    void kph();
    void oilTemp();
    void loop();
};
