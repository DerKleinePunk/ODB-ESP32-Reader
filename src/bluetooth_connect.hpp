#pragma once

//https://esp32-server.de/ibeacon-itag-praesenz-schalter/ 

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEAddress.h>

class BluetoothConnect
{
private:
    BLEScan* m_pBLEScan;
    u_int32_t m_nScanTime;
public:
    BluetoothConnect();
    ~BluetoothConnect();

    void Scan();
};


