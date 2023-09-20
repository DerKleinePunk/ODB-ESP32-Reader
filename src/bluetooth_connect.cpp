#include "bluetooth_connect.hpp"

#include "command_reader.hpp"

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    BluetoothConnect* _parent;

  public:
    MyAdvertisedDeviceCallbacks(BluetoothConnect* parent)
    {
        _parent = parent;
    }

    void onResult(BLEAdvertisedDevice advertisedDevice) // passiert wenn BLE Device ( beacon ) gefunden wurde
    {
        btDevice device;
        // Serial.print(advertisedDevice.getAddress().toString().c_str());
        if(advertisedDevice.haveName()) {
            device.name = advertisedDevice.getName().c_str();
        } else {
            device.name = F("not defined");
        }
        if(advertisedDevice.haveManufacturerData()) {
            /*Serial.print(" - ");
            char *pHex = BLEUtils::buildHexData(nullptr, (uint8_t*)advertisedDevice.getManufacturerData().data(),
            advertisedDevice.getManufacturerData().length()); Serial.print(pHex); free(pHex);*/
        }

        device.address = BTAddress(*(advertisedDevice.getAddress().getNative()));
        device.classic = false;
        _parent->m_pAddresses.push_back(device);
    }
};

BluetoothConnect::BluetoothConnect()
{
    BLEDevice::init("ODB-ESP");
    m_SerialBT.begin("ODB-ESP", true); // Bluetooth device name
    m_pBLEScan = nullptr;
    m_nScanTime = 5; // In seconds
    m_nCurrentDevice = 0;
}

BluetoothConnect::~BluetoothConnect()
{
}

void BluetoothConnect::Scan()
{
    if(m_pBLEScan == nullptr) {
        m_pBLEScan = BLEDevice::getScan(); // create new scan
        m_pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
        m_pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    }

    if(m_SerialBT.connected()) {
        m_SerialBT.disconnect();
        m_emlConnected = false;
    }

    m_pAddresses.clear();
    m_pBLEScan->start(m_nScanTime);
    BTScanResults* pResults = m_SerialBT.discover(m_nScanTime * 1000); // Scantime in ms
    if(pResults) {
        for(int i = 0; i < pResults->getCount(); i++) {
            btDevice device;
            device.address = pResults->getDevice(i)->getAddress();
            device.name = pResults->getDevice(i)->getName().c_str();
            device.classic = true;
            m_pAddresses.push_back(device);
        }
    }

    if(m_pAddresses.empty()) {
        Serial.println(F("No Devices found"));
    } else {
        auto counter = 0;
        for(const auto entry : m_pAddresses) {
            Serial.print(counter);
            Serial.print(" ");
            Serial.print(entry.address.toString());
            Serial.print(" ");
            Serial.print(entry.name);
            Serial.print(" ");
            Serial.print(entry.classic);
            Serial.println();
            counter++;
        }
    }
}

void BluetoothConnect::ConnectSerial(byte index)
{
    if(index >= m_pAddresses.size()) {
        log_d("index not in Array");
        return;
    }

    if(!m_pAddresses[index].classic) {
        log_d("not supported");
        return;
    }

    Serial.println("Try Connect to " + m_pAddresses[index].name);

    // Todo Make Configable
    m_SerialBT.setPin("1234");
    m_SerialBT.connect(m_pAddresses[index].address);
    if(m_SerialBT.connected(m_nScanTime * 1000)) {
        log_d("bt serial connected");
        // if(m_SerialBT.isReady(m_nScanTime * 1000)
        m_SerialBT.print("AT Z\r\n");
        m_SerialBT.flush();

        CommandReader commandReader;
        commandReader.begin(m_SerialBT);
        auto commands = commandReader.GetCommandTimeout(m_nScanTime * 1000);
        if(!commands.empty()) {
            for(const auto command : commands) {
                log_d("result %s", command);
                if(command.indexOf("ELM") > -1) {
                    m_emlConnected = true;
                }
            }
        }
        if(m_emlConnected) {
            Serial.println("ODBII found");
            if(m_ELMduino.begin(m_SerialBT, true, 2000)) {
                Serial.println("connected to OBD scanner - Phase 2");
            } else {
                Serial.println("failed to connect ODB Scanner");
                m_emlConnected = false;
            }
        }
    } else {
        log_d("bt serial not conneted");
    }
}

void BluetoothConnect::Disconnect()
{
    m_emlConnected = false;
    if(m_SerialBT.connected()) {
        m_SerialBT.disconnect();
    }
    /*m_SerialBT.end();
    BLEDevice::init("ODB-ESP");
    m_SerialBT.begin("ODB-ESP", true); // Bluetooth device name
    m_pBLEScan = nullptr;*/
}

float BluetoothConnect::rpm()
{
    if(!m_emlConnected) return 0.0;

    auto rpm = m_ELMduino.rpm();
    while(m_ELMduino.nb_rx_state != ELM_SUCCESS) {
        if(m_ELMduino.nb_rx_state != ELM_GETTING_MSG) {
            m_ELMduino.printError();
            break;
        }
        rpm = m_ELMduino.rpm();
    }

    if(m_ELMduino.nb_rx_state == ELM_SUCCESS) {
      rpm = m_ELMduino.rpm();
    }

    return rpm;
}