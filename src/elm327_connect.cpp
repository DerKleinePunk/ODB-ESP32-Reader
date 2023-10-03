#include "elm327_connect.hpp"
#include "command_reader.hpp"

#ifdef BLE_ENABLED    
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    Elm327Connect* _parent;
    
    // NOTE: disable copy constructor and assignment operator
    MyAdvertisedDeviceCallbacks(const MyAdvertisedDeviceCallbacks&) = delete;
    MyAdvertisedDeviceCallbacks& operator=(const MyAdvertisedDeviceCallbacks&) & = delete;

  public:
    explicit MyAdvertisedDeviceCallbacks(Elm327Connect* parent)
    {
        _parent = parent;
    }

    void onResult(BLEAdvertisedDevice advertisedDevice) override;
};

// passiert wenn BLE Device ( beacon ) gefunden wurde
void MyAdvertisedDeviceCallbacks::onResult(BLEAdvertisedDevice advertisedDevice) 
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
#endif

bool Elm327Connect::AddPidIfNotExits(obd_pid_states pid)
{
    auto found = false;
    for(auto i = 0; i < 20; i++) {
        if(_obd_pid_list[i] == obd_pid_states::NOTHING) {
            _obd_pid_list[i] = pid;
            found = true;
            break;
        }
        if(_obd_pid_list[i] == pid) {
            found = true;
            break;
        }
    }

    return found;
}

Elm327Connect::Elm327Connect() : 
#ifdef BLE_ENABLED    
    _pBLEScan(nullptr), 
#endif
    _emlConnected(false), 
    _event(nullptr)
{    
#ifdef BLE_ENABLED  
    BLEDevice::init("ODB-ESP");
#endif
    _SerialBT.begin("ODB-ESP", true); // Bluetooth device name
    _nScanTime = 5; // In seconds
    _current_obd_pid = obd_pid_states::NOTHING;
    for(auto i = 0; i < 20; i++) {
        _obd_pid_list[i] = obd_pid_states::NOTHING;
    }
}

Elm327Connect::~Elm327Connect()
{
}

void Elm327Connect::Scan()
{
#ifdef BLE_ENABLED  
    if(_pBLEScan == nullptr) {
        _pBLEScan = BLEDevice::getScan(); // create new scan
        _pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(this));
        _pBLEScan->setActiveScan(true); // active scan uses more power, but get results faster
    }
#endif
    if(_SerialBT.connected()) {
        _SerialBT.disconnect();
        _emlConnected = false;
    }

    m_pAddresses.clear();
#ifdef BLE_ENABLED
    _pBLEScan->start(_nScanTime);
#endif
    BTScanResults* pResults = _SerialBT.discover(_nScanTime * 1000); // Scantime in ms
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

void Elm327Connect::ConnectSerial(byte index)
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
    _SerialBT.setPin("1234");
    _SerialBT.connect(m_pAddresses[index].address);
    if(_SerialBT.connected(_nScanTime * 1000)) {
        log_d("bt serial connected");
        // if(m_SerialBT.isReady(m_nScanTime * 1000)
        _SerialBT.print("AT Z\r\n");
        _SerialBT.flush();

        CommandReader commandReader;
        commandReader.begin(_SerialBT);
        auto commands = commandReader.GetCommandTimeout(_nScanTime * 1000);
        if(!commands.empty()) {
            for(const auto command : commands) {
                log_d("result %s", command);
                if(command.indexOf("ELM") > -1) {
                    _emlConnected = true;
                }
            }
        }
        if(_emlConnected) {
            Serial.println("ODBII found");
            if(_ELMduino.begin(_SerialBT, true, 4000)) {
                Serial.println("connected to OBD scanner - Phase 2");
            } else {
                Serial.println("failed to connect ODB Scanner");
                _emlConnected = false;
            }
        }
    } else {
        log_d("bt serial not conneted");
    }
}

void Elm327Connect::Disconnect()
{
    _emlConnected = false;
    if(_SerialBT.connected()) {
        _SerialBT.disconnect();
    }
    /*m_SerialBT.end();
    BLEDevice::init("ODB-ESP");
    m_SerialBT.begin("ODB-ESP", true); // Bluetooth device name
    m_pBLEScan = nullptr;*/
}

void Elm327Connect::ValueChangedCallback(ValueChangedEvent callback)
{
    _event = callback;
}


void Elm327Connect::rpm()
{
    if(!_emlConnected) return;

    if(_current_obd_pid == obd_pid_states::NOTHING) {
        _current_obd_pid = obd_pid_states::RPM;
    } else {
        AddPidIfNotExits(obd_pid_states::RPM);
    }
}

void Elm327Connect::kph()
{
    if(!_emlConnected) return;

    if(_current_obd_pid == obd_pid_states::NOTHING) {
        _current_obd_pid = obd_pid_states::KPH;
    } else {
        AddPidIfNotExits(obd_pid_states::KPH);
    }
}

void Elm327Connect::oilTemp()
{
    if(!_emlConnected) return;

    if(_current_obd_pid == obd_pid_states::NOTHING) {
        _current_obd_pid = obd_pid_states::OILTEMP;
    } else {
        AddPidIfNotExits(obd_pid_states::OILTEMP);
    }
}

void Elm327Connect::loop()
{
    if(!_emlConnected) return;

    if(_current_obd_pid == obd_pid_states::NOTHING) return;

    switch(_current_obd_pid) {
    case obd_pid_states::RPM:
        _motor_State.rpm = _ELMduino.rpm();
        if(_ELMduino.nb_rx_state == ELM_SUCCESS) {
            if(_event != nullptr) {
                _event(_current_obd_pid, _motor_State);
            }
            _current_obd_pid = obd_pid_states::NOTHING;
        }
        break;
    case obd_pid_states::KPH:
        _motor_State.rpm = _ELMduino.kph();
        if(_ELMduino.nb_rx_state == ELM_SUCCESS) {
            if(_event != nullptr) {
                _event(_current_obd_pid, _motor_State);
            }
            _current_obd_pid = obd_pid_states::NOTHING;
        }
        break;
    case obd_pid_states::OILTEMP:
        _motor_State.rpm = _ELMduino.oilTemp();
        if(_ELMduino.nb_rx_state == ELM_SUCCESS) {
            if(_event != nullptr) {
                _event(_current_obd_pid, _motor_State);
            }
            _current_obd_pid = obd_pid_states::NOTHING;
        }
        break;
    default:
        log_e("_current_obd_pid not implemented");
        break;
    }

    if(_ELMduino.nb_rx_state != ELM_GETTING_MSG) {
        _ELMduino.printError();
        _current_obd_pid = obd_pid_states::NOTHING;
    }

    if(_ELMduino.nb_rx_state == ELM_SUCCESS) {
        if(_obd_pid_list[0] != obd_pid_states::NOTHING) {
            _current_obd_pid = _obd_pid_list[0];

            for(auto i = 0; i < 19; i++) {
                _obd_pid_list[i] == _obd_pid_list[i + 1];
            }
            _obd_pid_list[19] = obd_pid_states::NOTHING;
        }
    }
}