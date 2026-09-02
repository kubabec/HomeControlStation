#include "devices/WindowBlinder/WindowBlinder.hpp"

namespace
{
constexpr uint8_t UNUSED_PIN = 255;
constexpr uint16_t CONTROL_PAYLOAD_SIZE = 6;
uint16_t readU16(const uint8_t* source)
{
    return static_cast<uint16_t>(source[0]) | (static_cast<uint16_t>(source[1]) << 8);
}
}

WindowBlinder::WindowBlinder(DeviceConfigSlotType config)
    : upPin(config.pinNumber), downPin(config.customBytes[0]), upperLimitPin(config.customBytes[1]),
      lowerLimitPin(config.customBytes[2]), deviceId(config.deviceId), roomId(config.roomId),
      deviceName(config.deviceName), outputActiveHigh(config.customBytes[3] != 0),
      limitActiveHigh(config.customBytes[4] != 0),
      openTimeoutMs(static_cast<uint32_t>(readU16(&config.customBytes[5])) * 1000UL),
      closeTimeoutMs(static_cast<uint32_t>(readU16(&config.customBytes[7])) * 1000UL)
{
    if (openTimeoutMs == 0) openTimeoutMs = 30000;
    if (closeTimeoutMs == 0) closeTimeoutMs = 30000;
}

bool WindowBlinder::limitReached(uint8_t pin) const
{
    return pin != UNUSED_PIN && (digitalRead(pin) == (limitActiveHigh ? HIGH : LOW));
}

void WindowBlinder::writeMotor(uint8_t pin, bool active)
{
    digitalWrite(pin, (active == outputActiveHigh) ? HIGH : LOW);
}

void WindowBlinder::stop()
{
    writeMotor(upPin, false);
    writeMotor(downPin, false);
    motion = STOPPED;
}

bool WindowBlinder::start(Motion requestedMotion)
{
    stop();
    if (requestedMotion == OPENING && limitReached(upperLimitPin)) { positionPercent = 100; return false; }
    if (requestedMotion == CLOSING && limitReached(lowerLimitPin)) { positionPercent = 0; return false; }
    fault = 0;
    motion = requestedMotion;
    motionStartedAt = positionUpdatedAt = millis();
    writeMotor(requestedMotion == OPENING ? upPin : downPin, true);
    return true;
}

void WindowBlinder::updatePosition(unsigned long now)
{
    if (positionPercent == 255 || motion == STOPPED) { positionUpdatedAt = now; return; }
    const uint32_t duration = motion == OPENING ? openTimeoutMs : closeTimeoutMs;
    const uint32_t elapsed = min(static_cast<uint32_t>(now - positionUpdatedAt), duration);
    const uint8_t delta = static_cast<uint8_t>((elapsed * 100UL) / duration);
    if (delta == 0) return;
    positionUpdatedAt = now;
    if (motion == OPENING) positionPercent = min(100, positionPercent + delta);
    else positionPercent = delta >= positionPercent ? 0 : positionPercent - delta;
}

void WindowBlinder::init()
{
    digitalWrite(upPin, outputActiveHigh ? LOW : HIGH);
    digitalWrite(downPin, outputActiveHigh ? LOW : HIGH);
    pinMode(upPin, OUTPUT);
    pinMode(downPin, OUTPUT);
    if (upperLimitPin != UNUSED_PIN) pinMode(upperLimitPin, INPUT_PULLUP);
    if (lowerLimitPin != UNUSED_PIN) pinMode(lowerLimitPin, INPUT_PULLUP);
    if (limitReached(upperLimitPin)) positionPercent = 100;
    else if (limitReached(lowerLimitPin)) positionPercent = 0;
}

void WindowBlinder::cyclic()
{
    const unsigned long now = millis();
    const bool upper = limitReached(upperLimitPin);
    const bool lower = limitReached(lowerLimitPin);
    if (upper && lower) { fault = 2; stop(); return; }
    updatePosition(now);
    if (motion == OPENING && upper) { positionPercent = 100; stop(); return; }
    if (motion == CLOSING && lower) { positionPercent = 0; stop(); return; }
    const uint32_t timeout = motion == OPENING ? openTimeoutMs : closeTimeoutMs;
    if (motion != STOPPED && static_cast<uint32_t>(now - motionStartedAt) >= timeout)
    {
        fault = 1;
        stop();
    }
}

DeviceDescription WindowBlinder::getDeviceDescription()
{
    DeviceDescription description;
    description.deviceType = getDeviceType(); description.deviceId = deviceId; description.roomId = roomId;
    description.isEnabled = motion != STOPPED; description.deviceName = deviceName;
    description.customBytes[0] = motion; description.customBytes[1] = positionPercent;
    description.customBytes[2] = limitReached(upperLimitPin); description.customBytes[3] = limitReached(lowerLimitPin);
    description.customBytes[4] = fault;
    return description;
}

uint8_t WindowBlinder::getDeviceIdentifier() { return deviceId; }
uint8_t WindowBlinder::getDeviceType() { return type_WINDOW_BLINDER; }
uint16_t WindowBlinder::getExtendedMemoryLength() { return 0; }
ServiceRequestErrorCode WindowBlinder::service(DeviceServicesType) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode WindowBlinder::service(DeviceServicesType, ServiceParameters_set1) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode WindowBlinder::service(DeviceServicesType, ServiceParameters_set2) { return SERV_NOT_SUPPORTED; }

ServiceRequestErrorCode WindowBlinder::service(DeviceServicesType serviceType, ServiceParameters_set3 parameters)
{
    if (serviceType == DEVSERVICE_GET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != CONTROL_PAYLOAD_SIZE || parameters.direction != e_OUT_from_DEVICE) return SERV_EXECUTION_FAILURE;
        parameters.buff[0] = 0; parameters.buff[1] = motion; parameters.buff[2] = positionPercent;
        parameters.buff[3] = limitReached(upperLimitPin); parameters.buff[4] = limitReached(lowerLimitPin); parameters.buff[5] = fault;
        return SERV_SUCCESS;
    }
    if (serviceType == DEVSERVICE_SET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != CONTROL_PAYLOAD_SIZE || parameters.direction != e_IN_to_DEVICE || parameters.buff[0] > 2) return SERV_EXECUTION_FAILURE;
        if (parameters.buff[0] == 0) stop();
        else start(parameters.buff[0] == 1 ? OPENING : CLOSING);
        return SERV_SUCCESS;
    }
    return SERV_NOT_SUPPORTED;
}