#pragma once

#include <vector>
#include <string>
#include "device.hpp"

namespace HA
{
    
struct mqttValue {
    std::string topic;
    std::string payload;
};

class sensor
{
  private:
    std::vector<device> _devices;
    std::string _name;
    std::string _deviceId;
  public:
    // NOTE: disable copy constructor and assignment operator
    sensor(const sensor&) = delete;
    sensor& operator=(const sensor&) & = delete;

    explicit sensor(const std::string& name, const std::string& deviceId);
    ~sensor();

    void addDevice(device& device);

    std::vector<mqttValue> getConfigJson();
};
} // namespace HA

