#pragma once

#include <string>

namespace HA {
    enum struct device_types { TEMPERATURE, HUMIDITY, BINARY_SENSOR, SWITCH };

    class device
    {
    private:
        std::string _name;
        device_types _deviceType;
    public:
        // NOTE: disable copy constructor and assignment operator
        //device(const device&) = delete; vector need this
        device& operator=(const device& other); 

        explicit device(const std::string name, device_types type);
        ~device();

        std::string getName() const;
        std::string getConfigJson(const std::string& treeName, const std::string& deviceId, const std::string& sensorName) const;
    };
}