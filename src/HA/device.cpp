#include "device.hpp"

#include <ArduinoJson.h>

#include <sstream> // std::stringstream, std::stringbuf


namespace HA
{

device::device(const std::string name, device_types type)
{
    _name = name;
    _deviceType = type;
}

device::~device()
{
}

std::string device::getName() const
{
    return _name;
}

std::string device::getConfigJson(const std::string& treeName, const std::string& deviceId, const std::string& sensorName) const
{
    StaticJsonDocument<384> doc;

    switch(_deviceType) {
    case device_types::TEMPERATURE:
        doc[F("device_class")] = "temperature";
        doc[F("unit_of_measurement")] = "°C";
        break;
    case device_types::SWITCH:
        doc[F("device_class")] = "switch";
        doc[F("cmd_t")] = "~/set";
        break;

    default:
        log_e("missing type implementation %d", _deviceType);
        break;
    }

    doc[F("~")] = "homeassistant/sensor/" + treeName;
    doc[F("state_t")] = "~/state";
    
    doc[F("value_template")] = "{{ value_json." + _name + "}}";
    doc[F("unique_id")] = _name + deviceId;

    JsonObject device = doc.createNestedObject("device");
    device["identifiers"][0] = sensorName + deviceId;
    device["name"] = sensorName;

    if(doc.overflowed()) {
        log_e("json memory to small");
    }

    std::stringstream ss;
    serializeJson(doc, ss);

    return ss.str();
}
} // namespace HA