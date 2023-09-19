#include "bluetooth_connect.hpp"

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
    BluetoothConnect* _parent;

  public:

    MyAdvertisedDeviceCallbacks(BluetoothConnect* parent)
    {
      _parent = parent;
    }

    void onResult(BLEAdvertisedDevice advertisedDevice) // passiert wenn BLE Device ( beacon ) gefunden wurde
    {
      Serial.print(advertisedDevice.getAddress().toString().c_str());
      if(advertisedDevice.haveName()){
        Serial.print(" - ");
        Serial.print(advertisedDevice.getName().c_str());
      }
      if(advertisedDevice.haveManufacturerData()){
        Serial.print(" - ");
        Serial.print(advertisedDevice.getManufacturerData().c_str());
      }
      Serial.println();

      _parent->m_pAddresses.push_back(advertisedDevice.getAddress());
    }
};

BluetoothConnect::BluetoothConnect()
{
    BLEDevice::init("ODB-ESP");
    m_SerialBT.begin("ODB-ESP"); //Bluetooth device name
    m_pBLEScan = nullptr;
    m_nScanTime = 5; //In seconds
    m_nCurrentDevice = 0;
}

BluetoothConnect::~BluetoothConnect()
{
}

void BluetoothConnect::Scan() {
    if(m_pBLEScan == nullptr) {
        m_pBLEScan = BLEDevice::getScan(); //create new scan
        m_pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
        m_pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
    }

    m_pAddresses.clear();
    m_pBLEScan->start(m_nScanTime);
    BTScanResults *pResults = m_SerialBT.discover(m_nScanTime* 1000);//Scantime in ms
    if (pResults) {
      pResults->dump(&Serial);
    } else {
      Serial.println("no classic device found");
    }
} 