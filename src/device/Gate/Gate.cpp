#include "devices/Gate/Gate.hpp"

namespace { constexpr uint8_t UNUSED_PIN = 255; constexpr uint16_t PAYLOAD_SIZE = 5; constexpr uint32_t INTERLOCK_MS = 250; }

Gate::Gate(DeviceConfigSlotType config)
    : motor1OpenPin(config.pinNumber), motor1ClosePin(config.customBytes[0]), motor2OpenPin(config.customBytes[1]),
      motor2ClosePin(config.customBytes[2]), openedLimitPin(config.customBytes[3]), closedLimitPin(config.customBytes[4]),
      deviceId(config.deviceId), roomId(config.roomId), deviceName(config.deviceName),
      motorCount(config.customBytes[5] == 2 ? 2 : 1), outputActiveHigh(config.customBytes[6] != 0),
      travelTimeoutMs((static_cast<uint32_t>(config.customBytes[7]) | (static_cast<uint32_t>(config.customBytes[8]) << 8)) * 1000UL)
{
    if (travelTimeoutMs == 0) travelTimeoutMs = 60000;
}

bool Gate::limitReached(uint8_t pin) const { return pin != UNUSED_PIN && digitalRead(pin) == LOW; }
void Gate::writeOutput(uint8_t pin, bool active) { if (pin != UNUSED_PIN) digitalWrite(pin, (active == outputActiveHigh) ? HIGH : LOW); }

void Gate::deactivateOutputs()
{
    writeOutput(motor1OpenPin, false); writeOutput(motor1ClosePin, false);
    if (motorCount == 2) { writeOutput(motor2OpenPin, false); writeOutput(motor2ClosePin, false); }
}

void Gate::stop() { deactivateOutputs(); motion = STOPPED; pendingMotion = STOPPED; }

void Gate::request(Motion requestedMotion)
{
    stop();
    if ((requestedMotion == OPENING && limitReached(openedLimitPin)) || (requestedMotion == CLOSING && limitReached(closedLimitPin))) return;
    pendingMotion = requestedMotion;
    transitionStartedAt = millis();
    fault = 0;
}

void Gate::activatePending(unsigned long now)
{
    if (pendingMotion == STOPPED || static_cast<uint32_t>(now - transitionStartedAt) < INTERLOCK_MS) return;
    motion = pendingMotion; pendingMotion = STOPPED; motionStartedAt = now;
    const bool opening = motion == OPENING;
    writeOutput(opening ? motor1OpenPin : motor1ClosePin, true);
    if (motorCount == 2) writeOutput(opening ? motor2OpenPin : motor2ClosePin, true);
}

void Gate::init()
{
    const uint8_t pins[] = {motor1OpenPin, motor1ClosePin, motor2OpenPin, motor2ClosePin};
    for (uint8_t index = 0; index < 4; ++index)
    {
        if (pins[index] == UNUSED_PIN || (index >= 2 && motorCount == 1)) continue;
        digitalWrite(pins[index], outputActiveHigh ? LOW : HIGH);
        pinMode(pins[index], OUTPUT);
    }
    if (openedLimitPin != UNUSED_PIN) pinMode(openedLimitPin, INPUT_PULLUP);
    if (closedLimitPin != UNUSED_PIN) pinMode(closedLimitPin, INPUT_PULLUP);
}

void Gate::cyclic()
{
    const unsigned long now = millis();
    const bool opened = limitReached(openedLimitPin);
    const bool closed = limitReached(closedLimitPin);
    if (opened && closed) { fault = 2; stop(); return; }
    activatePending(now);
    if ((motion == OPENING && opened) || (motion == CLOSING && closed)) { stop(); return; }
    if (motion != STOPPED && static_cast<uint32_t>(now - motionStartedAt) >= travelTimeoutMs) { fault = 1; stop(); }
}

DeviceDescription Gate::getDeviceDescription()
{
    DeviceDescription description;
    description.deviceType = getDeviceType(); description.deviceId = deviceId; description.roomId = roomId;
    description.isEnabled = motion != STOPPED || pendingMotion != STOPPED; description.deviceName = deviceName;
    description.customBytes[0] = pendingMotion != STOPPED ? pendingMotion : motion;
    description.customBytes[1] = limitReached(openedLimitPin); description.customBytes[2] = limitReached(closedLimitPin);
    description.customBytes[3] = fault; description.customBytes[4] = motorCount;
    return description;
}

uint8_t Gate::getDeviceIdentifier() { return deviceId; }
uint8_t Gate::getDeviceType() { return type_GATE; }
uint16_t Gate::getExtendedMemoryLength() { return 0; }
ServiceRequestErrorCode Gate::service(DeviceServicesType) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode Gate::service(DeviceServicesType, ServiceParameters_set1) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode Gate::service(DeviceServicesType, ServiceParameters_set2) { return SERV_NOT_SUPPORTED; }

ServiceRequestErrorCode Gate::service(DeviceServicesType serviceType, ServiceParameters_set3 parameters)
{
    if (serviceType == DEVSERVICE_GET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != PAYLOAD_SIZE || parameters.direction != e_OUT_from_DEVICE) return SERV_EXECUTION_FAILURE;
        parameters.buff[0] = 0; parameters.buff[1] = pendingMotion != STOPPED ? pendingMotion : motion;
        parameters.buff[2] = limitReached(openedLimitPin); parameters.buff[3] = limitReached(closedLimitPin); parameters.buff[4] = fault;
        return SERV_SUCCESS;
    }
    if (serviceType == DEVSERVICE_SET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != PAYLOAD_SIZE || parameters.direction != e_IN_to_DEVICE || parameters.buff[0] > 2) return SERV_EXECUTION_FAILURE;
        if (parameters.buff[0] == 0) stop(); else request(parameters.buff[0] == 1 ? OPENING : CLOSING);
        return SERV_SUCCESS;
    }
    return SERV_NOT_SUPPORTED;
}