/** @file Gate.hpp @brief Single- or double-motor gate controller. */
#ifndef GATE_HPP
#define GATE_HPP

#include "Arduino.h"
#include "devices/device.hpp"

class Gate : public Device
{
private:
    enum Motion : uint8_t { STOPPED = 0, OPENING = 1, CLOSING = 2 };
    uint8_t motor1OpenPin;
    uint8_t motor1ClosePin;
    uint8_t motor2OpenPin;
    uint8_t motor2ClosePin;
    uint8_t openedLimitPin;
    uint8_t closedLimitPin;
    uint8_t deviceId;
    uint8_t roomId;
    String deviceName;
    uint8_t motorCount;
    bool outputActiveHigh;
    uint32_t travelTimeoutMs;
    Motion motion = STOPPED;
    Motion pendingMotion = STOPPED;
    uint8_t fault = 0;
    unsigned long transitionStartedAt = 0;
    unsigned long motionStartedAt = 0;

    bool limitReached(uint8_t pin) const;
    void writeOutput(uint8_t pin, bool active);
    void deactivateOutputs();
    void stop();
    void request(Motion requestedMotion);
    void activatePending(unsigned long now);

public:
    explicit Gate(DeviceConfigSlotType config);
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