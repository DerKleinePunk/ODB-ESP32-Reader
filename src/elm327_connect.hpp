#pragma once
#include <Arduino.h>
#include <BluetoothSerial.h>
// https://esp32-server.de/ibeacon-itag-praesenz-schalter/

#ifdef BLE_ENABLED
#include <BLEAddress.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#endif

#include <vector>

#include "ELMduino.h"

enum struct obd_pid_states { NOTHING, COOLANT, OILTEMP, INTAKE_AIR, RPM, KPH, SUPPORTEDPID_1_20, MONITORSTATUS };

struct btDevice {
    BTAddress address;
    String name;
    bool classic;
};

struct MotorState {
    MotorState() {
        rpm = -1.0;
        kph = -1;
        oilTemp = -1.0;
        engineCoolantTemp = -1.0;
        intakeAirTemp = -1.0;
        pid1_20 = 0;
        motorState = 0;
    }
    float rpm;
    int32_t kph;
    float oilTemp;
    float engineCoolantTemp;
    float intakeAirTemp;
    uint32_t pid1_20;
    uint32_t motorState;
};


class Elm327Connect
{
    using ValueChangedEvent = void (*)(obd_pid_states, MotorState);

  private:
#ifdef BLE_ENABLED
    BLEScan* _pBLEScan;
#endif
    uint32_t _nScanTime;
    BluetoothSerial _SerialBT;
    bool _emlConnected;
    ELM327 _ELMduino;
    obd_pid_states _current_obd_pid;
    obd_pid_states _obd_pid_list[20];
    ValueChangedEvent _event;
    MotorState _motor_State;
    bool AddPidIfNotExits(obd_pid_states pid);

  public:
    Elm327Connect();
    ~Elm327Connect();

    std::vector<btDevice> m_pAddresses;

    void Scan();
    void ConnectSerial(byte index);
    void Disconnect();
    void ValueChangedCallback(ValueChangedEvent callback);
    void coolantTemp();
    void oilTemp();
    void intakeAir();
    void rpm();
    void kph();
    void supportedPID();
    void monitorStatus();
    void loop();
};
