#pragma once
#include <Arduino.h>
#include <BluetoothSerial.h>
//https://esp32-server.de/ibeacon-itag-praesenz-schalter/ 

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>
#include <vector>

class BluetoothConnect
{
private:
    BLEScan* m_pBLEScan;
    u_int32_t m_nScanTime;
    BluetoothSerial m_SerialBT;
public:
    BluetoothConnect();
    ~BluetoothConnect();

    void Scan();

    byte m_nCurrentDevice;
    std::vector<BLEAddress> m_pAddresses;
};


