#include "devices/AquariumController/AquariumController.hpp"
#include <cmath>

namespace
{
constexpr uint8_t UNUSED_PIN = 255;
constexpr uint16_t PAYLOAD_SIZE = 10;
constexpr float SERIES_RESISTOR_OHMS = 10000.0f;
constexpr float NOMINAL_RESISTANCE_OHMS = 10000.0f;
constexpr float NOMINAL_TEMPERATURE_K = 298.15f;
constexpr float BETA = 3950.0f;
uint16_t readU16(const uint8_t* data) { return static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8); }
}

AquariumController::AquariumController(DeviceConfigSlotType config)
    : temperaturePin(config.pinNumber), heaterPin(config.customBytes[0]), lightPin(config.customBytes[1]),
      filterPin(config.customBytes[2]), lowWaterPin(config.customBytes[3]), deviceId(config.deviceId), roomId(config.roomId),
      deviceName(config.deviceName), outputActiveHigh(config.customBytes[4] != 0),
      setpointC(readU16(&config.customBytes[5]) / 10.0f), hysteresisC(readU16(&config.customBytes[7]) / 10.0f),
      maximumTemperatureC(readU16(&config.customBytes[9]) / 10.0f)
{
    if (setpointC < 5.0f || setpointC > 40.0f) setpointC = 25.0f;
    if (hysteresisC < 0.1f || hysteresisC > 5.0f) hysteresisC = 0.5f;
    if (maximumTemperatureC < setpointC || maximumTemperatureC > 60.0f) maximumTemperatureC = 32.0f;
}

bool AquariumController::waterLow() const { return lowWaterPin != UNUSED_PIN && digitalRead(lowWaterPin) == LOW; }
void AquariumController::writeOutput(uint8_t pin, bool active) { digitalWrite(pin, (active == outputActiveHigh) ? HIGH : LOW); }

bool AquariumController::sampleTemperature()
{
    const int adc = analogRead(temperaturePin);
    if (adc <= 5 || adc >= 4090) return false;
    const float resistance = SERIES_RESISTOR_OHMS * static_cast<float>(adc) / static_cast<float>(4095 - adc);
    const float inverseKelvin = (1.0f / NOMINAL_TEMPERATURE_K) + (logf(resistance / NOMINAL_RESISTANCE_OHMS) / BETA);
    const float sample = (1.0f / inverseKelvin) - 273.15f;
    if (!std::isfinite(sample) || sample < -20.0f || sample > 80.0f) return false;
    temperatureC = sample;
    return true;
}

void AquariumController::updateHeater()
{
    fault = 0;
    if (!temperatureValid) fault |= 1;
    if (waterLow()) fault |= 2;
    if (temperatureValid && temperatureC >= maximumTemperatureC) fault |= 4;
    if (fault != 0 || heaterMode == HEATER_OFF) heaterOn = false;
    else if (heaterMode == HEATER_FORCED_ON) heaterOn = true;
    else if (temperatureC <= setpointC - hysteresisC) heaterOn = true;
    else if (temperatureC >= setpointC + hysteresisC) heaterOn = false;
    writeOutput(heaterPin, heaterOn);
}

void AquariumController::init()
{
    const uint8_t outputs[] = {heaterPin, lightPin, filterPin};
    for (uint8_t pin : outputs) { digitalWrite(pin, outputActiveHigh ? LOW : HIGH); pinMode(pin, OUTPUT); }
    if (lowWaterPin != UNUSED_PIN) pinMode(lowWaterPin, INPUT_PULLUP);
    writeOutput(filterPin, filterOn);
    temperatureValid = sampleTemperature();
    updateHeater();
    lastSampleAt = millis();
}

void AquariumController::cyclic()
{
    const unsigned long now = millis();
    if (static_cast<uint32_t>(now - lastSampleAt) < 1000) return;
    lastSampleAt = now;
    temperatureValid = sampleTemperature();
    updateHeater();
}

DeviceDescription AquariumController::getDeviceDescription()
{
    DeviceDescription description;
    description.deviceType = getDeviceType(); description.deviceId = deviceId; description.roomId = roomId;
    description.isEnabled = heaterOn || lightOn || filterOn; description.deviceName = deviceName;
    memcpy(&description.customBytes[0], &temperatureC, sizeof(temperatureC));
    description.customBytes[4] = temperatureValid; description.customBytes[5] = waterLow(); description.customBytes[6] = heaterOn;
    description.customBytes[7] = lightOn; description.customBytes[8] = filterOn; description.customBytes[9] = heaterMode; description.customBytes[10] = fault;
    return description;
}

uint8_t AquariumController::getDeviceIdentifier() { return deviceId; }
uint8_t AquariumController::getDeviceType() { return type_AQUARIUM_CONTROLLER; }
uint16_t AquariumController::getExtendedMemoryLength() { return 0; }
ServiceRequestErrorCode AquariumController::service(DeviceServicesType) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode AquariumController::service(DeviceServicesType, ServiceParameters_set1) { return SERV_NOT_SUPPORTED; }
ServiceRequestErrorCode AquariumController::service(DeviceServicesType, ServiceParameters_set2) { return SERV_NOT_SUPPORTED; }

ServiceRequestErrorCode AquariumController::service(DeviceServicesType serviceType, ServiceParameters_set3 parameters)
{
    if (serviceType == DEVSERVICE_GET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != PAYLOAD_SIZE || parameters.direction != e_OUT_from_DEVICE) return SERV_EXECUTION_FAILURE;
        const int16_t temperatureTenths = temperatureValid ? static_cast<int16_t>(roundf(temperatureC * 10.0f)) : 0;
        parameters.buff[0] = heaterMode; parameters.buff[1] = lightOn; parameters.buff[2] = filterOn; parameters.buff[3] = heaterOn;
        parameters.buff[4] = temperatureValid; parameters.buff[5] = waterLow();
        parameters.buff[6] = static_cast<uint8_t>(temperatureTenths & 0xFF); parameters.buff[7] = static_cast<uint8_t>((temperatureTenths >> 8) & 0xFF);
        parameters.buff[8] = fault; parameters.buff[9] = 0;
        return SERV_SUCCESS;
    }
    if (serviceType == DEVSERVICE_SET_ADVANCED_CONTROLS)
    {
        if (!parameters.buff || parameters.size != PAYLOAD_SIZE || parameters.direction != e_IN_to_DEVICE || parameters.buff[0] > 2 || parameters.buff[1] > 1 || parameters.buff[2] > 1) return SERV_EXECUTION_FAILURE;
        heaterMode = static_cast<HeaterMode>(parameters.buff[0]); lightOn = parameters.buff[1] != 0; filterOn = parameters.buff[2] != 0;
        writeOutput(lightPin, lightOn); writeOutput(filterPin, filterOn); updateHeater();
        return SERV_SUCCESS;
    }
    return SERV_NOT_SUPPORTED;
}