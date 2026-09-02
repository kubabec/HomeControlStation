/** @file AquariumController.hpp @brief Integrated aquarium temperature and equipment controller. */
#ifndef AQUARIUM_CONTROLLER_HPP
#define AQUARIUM_CONTROLLER_HPP

#include "Arduino.h"
#include "devices/device.hpp"

class AquariumController : public Device
{
private:
    enum HeaterMode : uint8_t { HEATER_OFF = 0, HEATER_AUTO = 1, HEATER_FORCED_ON = 2 };
    uint8_t temperaturePin;
    uint8_t heaterPin;
    uint8_t lightPin;
    uint8_t filterPin;
    uint8_t lowWaterPin;
    uint8_t deviceId;
    uint8_t roomId;
    String deviceName;
    bool outputActiveHigh;
    float setpointC;
    float hysteresisC;
    float maximumTemperatureC;
    float temperatureC = 0.0f;
    bool temperatureValid = false;
    bool heaterOn = false;
    bool lightOn = false;
    bool filterOn = true;
    HeaterMode heaterMode = HEATER_AUTO;
    uint8_t fault = 0;
    unsigned long lastSampleAt = 0;

    bool waterLow() const;
    void writeOutput(uint8_t pin, bool active);
    bool sampleTemperature();
    void updateHeater();

public:
    explicit AquariumController(DeviceConfigSlotType config);
    void init() override;
    void cyclic() override;
    DeviceDescription getDeviceDescription() override;
    uint8_t getDeviceIdentifier() override;
    uint8_t getDeviceType() override;
    uint16_t getExtendedMemoryLength() override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 parameters) override;
    ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 parameters) override;
};

#endif