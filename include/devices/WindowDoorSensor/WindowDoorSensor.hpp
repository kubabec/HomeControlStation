/** @file WindowDoorSensor.hpp @brief Debounced magnetic contact sensor. */
#ifndef WINDOW_DOOR_SENSOR_HPP
#define WINDOW_DOOR_SENSOR_HPP

#include "Arduino.h"
#include "devices/device.hpp"
#include <functional>

class WindowDoorSensor : public Device
{
private:
    uint8_t sensorPin;
    uint8_t deviceId;
    uint8_t roomId;
    String deviceName;
    bool closedActiveHigh;
    bool internalPullup;
    uint16_t debounceMs;
    bool rawClosed = false;
    bool stableClosed = false;
    unsigned long rawChangedAt = 0;
    std::function<void(uint8_t)> fireDeviceEvent;

    bool readClosed() const;

public:
    WindowDoorSensor(DeviceConfigSlotType config, std::function<void(uint8_t)> fireDeviceEventCallback);
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