#include "sensor.hpp"

namespace HA
{
sensor::sensor(const std::string& name, const std::string& deviceId)
{
    _name = name;
    _deviceId = deviceId;
}

sensor::~sensor()
{
}

void sensor::addDevice(device& device)
{
    _devices.push_back(device);
}

std::vector<mqttValue> sensor::getConfigJson()
{
    std::vector<mqttValue> result;

    for(auto device : _devices) {
        mqttValue entry;
        const auto treeName = _name + device.getName();
        entry.topic = "homeassistant/sensor/" + treeName + "/config";
        entry.payload = device.getConfigJson(treeName, _deviceId, _name);
        result.push_back(entry);
    }

    return result;
}

} // namespace HA