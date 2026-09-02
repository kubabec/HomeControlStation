/** @file WindowBlinder.hpp @brief Two-direction blind or shutter motor controller. */
#ifndef WINDOW_BLINDER_HPP
#define WINDOW_BLINDER_HPP

#include "Arduino.h"
#include "devices/device.hpp"

class WindowBlinder : public Device
{
private:
    enum Motion : uint8_t { STOPPED = 0, OPENING = 1, CLOSING = 2 };
    uint8_t upPin;
    uint8_t downPin;
    uint8_t upperLimitPin;
    uint8_t lowerLimitPin;
    uint8_t deviceId;
    uint8_t roomId;
    String deviceName;
    bool outputActiveHigh;
    bool limitActiveHigh;
    uint32_t openTimeoutMs;
    uint32_t closeTimeoutMs;
    Motion motion = STOPPED;
    uint8_t positionPercent = 255;
    uint8_t fault = 0;
    unsigned long motionStartedAt = 0;
    unsigned long positionUpdatedAt = 0;

    bool limitReached(uint8_t pin) const;
    void writeMotor(uint8_t pin, bool active);
    void stop();
    bool start(Motion requestedMotion);
    void updatePosition(unsigned long now);

public:
    explicit WindowBlinder(DeviceConfigSlotType config);
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