#include "devices/WindowDoorSensor/WindowDoorSensor.hpp"

WindowDoorSensor::WindowDoorSensor(DeviceConfigSlotType config)
    : sensorPin(config.pinNumber), deviceId(config.deviceId), roomId(config.roomId), deviceName(config.deviceName),
      closedActiveHigh(config.customBytes[0] != 0), internalPullup(config.customBytes[1] != 0),
      debounceMs(static_cast<uint16_t>(config.customBytes[2]) | (static_cast<uint16_t>(config.customBytes[3]) << 8))
{
    if (debounceMs == 0) debounceMs = 50;
}

bool WindowDoorSensor::readClosed() const
{
    return digitalRead(sensorPin) == (closedActiveHigh ? HIGH : LOW);
}

void WindowDoorSensor::init()
{
    pinMode(sensorPin, internalPullup ? INPUT_PULLUP : INPUT);
    rawClosed = stableClosed = readClosed();
    rawChangedAt = millis();
}

void WindowDoorSensor::cyclic()
{
    const bool sample = readClosed();
    const unsigned long now = millis();
    if (sample != rawClosed)
    {
        rawClosed = sample;
        rawChangedAt = now;
    }
    else if (stableClosed != rawClosed && static_cast<uint32_t>(now - rawChangedAt) >= debounceMs)
    {
        stableClosed = rawClosed;
    }
}

DeviceDescription WindowDoorSensor::getDeviceDescription()
{
    DeviceDescription description;
    description.deviceType = getDeviceType(); description.deviceId = deviceId; description.roomId = roomId;
    description.isEnabled = 2; description.deviceName = deviceName;
    description.customBytes[0] = stableClosed; description.customBytes[1] = rawClosed;
    return description;
}

uint8_t WindowDoorSensor::getDeviceIdentifier() { return deviceId; }
uint8_t WindowDoorSensor::getDeviceType() { return type_WINDOW_DOOR_SENSOR; }
uint16_t WindowDoorSensor::getExtendedMemoryLength() { return 0; }
ServiceRequestErrorCode WindowDoorSensor::service(DeviceServicesType) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode WindowDoorSensor::service(DeviceServicesType, ServiceParameters_set1) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode WindowDoorSensor::service(DeviceServicesType, ServiceParameters_set2) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode WindowDoorSensor::service(DeviceServicesType, ServiceParameters_set3) { return SERV_NOT_SUPPORTED; }