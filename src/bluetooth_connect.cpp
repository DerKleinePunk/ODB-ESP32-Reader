#include <Arduino.h>
#include "bluetooth_connect.hpp"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice) // passiert wenn BLE Device ( beacon ) gefunden wurde
    {
      Serial.print(advertisedDevice.getAddress().toString().c_str());
      Serial.print(" - ");
      Serial.print(advertisedDevice.getName().c_str());
      if(advertisedDevice.haveManufacturerData()){
        Serial.print(" - ");
        Serial.print(advertisedDevice.getManufacturerData().c_str());
      }
      Serial.println();
    }
};

BluetoothConnect::BluetoothConnect()
{
    BLEDevice::init("ODB-ESP");
    m_pBLEScan = nullptr;
    m_nScanTime = 30; //In seconds
}

BluetoothConnect::~BluetoothConnect()
{
}

void BluetoothConnect::Scan() {
    if(m_pBLEScan == nullptr) {
        m_pBLEScan = BLEDevice::getScan(); //create new scan
        m_pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
        m_pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    }

    m_pBLEScan->start(m_nScanTime);
} 