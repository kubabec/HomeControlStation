#ifndef GARDEN_IRRIGATION_DEVICE_HPP
#define GARDEN_IRRIGATION_DEVICE_HPP

#include "devices/device.hpp"
#include "Arduino.h"
#include <DHT.h>

class GardenIrrigationDevice : public Device
{
private:

    // ============================================================
    // DEVICE IDENTITY
    // ============================================================

    uint8_t deviceId = 255;
    uint8_t roomId = 255;
    bool isActive = false;
    String deviceName;

    // ============================================================
    // HARDWARE CONFIGURATION
    // ============================================================

    // DHT22 sensor pin.
    uint8_t sensorPin = 255;

    // Irrigation nozzle outputs.
    uint8_t nozzle1Pin = 255;
    uint8_t nozzle2Pin = 255;

    // Relay polarity.
    bool outputsActiveHigh = true;
    bool sensorEnabled = false;
    bool hardwareConfigurationValid = false;

    // ============================================================
    // SENSOR
    // ============================================================

    DHT *dht = nullptr;

    float temperature = NAN;
    float humidity = NAN;

    bool sensorError = false;

    // ============================================================
    // IRRIGATION STATE
    // ============================================================

    bool nozzle1On = false;
    bool nozzle2On = false;

    unsigned long nozzle1StartedAt = 0;
    unsigned long nozzle2StartedAt = 0;

    // ============================================================
    // ADVANCED CONFIGURATION
    // ============================================================

    // Watering duration in seconds.
    uint16_t wateringDurationSeconds = 60;

    // Humidity below this value means DRY.
    uint8_t dryThreshold = 40;

    // Humidity above this value means WET.
    uint8_t wetThreshold = 70;

    // ============================================================
    // INTERNAL HELPERS
    // ============================================================

    void readSensor();

    void setNozzle(uint8_t nozzle, bool state);

    void updateAutomaticStop();

    bool isDry() const;

    bool isWet() const;

public:

    explicit GardenIrrigationDevice(DeviceConfigSlotType config);

    ~GardenIrrigationDevice() override;

    void init() override;

    void cyclic() override;

    DeviceDescription getDeviceDescription() override;

    uint8_t getDeviceIdentifier() override;

    uint8_t getDeviceType() override;

    uint16_t getExtendedMemoryLength() override;

    ServiceRequestErrorCode service(
        DeviceServicesType serviceType
    ) override;

    ServiceRequestErrorCode service(
        DeviceServicesType serviceType,
        ServiceParameters_set1 parameters
    ) override;

    ServiceRequestErrorCode service(
        DeviceServicesType serviceType,
        ServiceParameters_set2 parameters
    ) override;

    ServiceRequestErrorCode service(
        DeviceServicesType serviceType,
        ServiceParameters_set3 parameters
    ) override;
};

#endif