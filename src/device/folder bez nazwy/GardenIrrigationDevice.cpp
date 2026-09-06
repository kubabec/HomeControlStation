#include "devices/GardenIrrigationDevice/GardenIrrigationDevice.hpp"

#include "generated/GeneratedEnablingConditions.hpp"

#include <math.h>

namespace
{
bool isUsableGpio(uint8_t pin)
{
    return pin > 0 && pin < 49 && !(pin >= 22 && pin <= 32);
}
}


// ================================================================
// CONFIGURATION BYTE LAYOUT
// ================================================================
//
// customBytes[0]  nozzle1 GPIO
// customBytes[1]  nozzle2 GPIO
// customBytes[2]  output polarity
//
// customBytes[3]  watering duration LSB
// customBytes[4]  watering duration MSB
//
// customBytes[5]  dry threshold
// customBytes[6]  wet threshold
//
// ================================================================


GardenIrrigationDevice::GardenIrrigationDevice(
    DeviceConfigSlotType config
)
{
    isActive = config.isActive;
    deviceId = config.deviceId;
    roomId = config.roomId;
    deviceName = String(config.deviceName);

    // The common pinNumber is used as the DHT22 pin.
    sensorPin = config.pinNumber;

    nozzle1Pin = config.customBytes[0];
    nozzle2Pin = config.customBytes[1];

    outputsActiveHigh = config.customBytes[2] != 0;
    sensorEnabled = config.customBytes[7] != 0;

    wateringDurationSeconds =
        static_cast<uint16_t>(config.customBytes[3]) |
        (static_cast<uint16_t>(config.customBytes[4]) << 8);

    if (wateringDurationSeconds == 0)
    {
        wateringDurationSeconds = 60;
    }

    dryThreshold = config.customBytes[5];
    wetThreshold = config.customBytes[6];

    // Reasonable defaults for fresh configuration.
    if (dryThreshold == 0 && wetThreshold == 0)
    {
        dryThreshold = 40;
        wetThreshold = 70;
    }

    if (dryThreshold >= wetThreshold)
    {
        dryThreshold = 40;
        wetThreshold = 70;
    }

    hardwareConfigurationValid =
        isUsableGpio(nozzle1Pin) &&
        isUsableGpio(nozzle2Pin) &&
        nozzle1Pin != nozzle2Pin;

    if (hardwareConfigurationValid && sensorEnabled && isUsableGpio(sensorPin))
    {
        dht = new DHT(sensorPin, DHT22);
    }
}

GardenIrrigationDevice::~GardenIrrigationDevice()
{
    delete dht;
}


// ================================================================
// INITIALIZATION
// ================================================================

void GardenIrrigationDevice::init()
{
    if (!hardwareConfigurationValid)
    {
        sensorError = true;
        return;
    }

    pinMode(nozzle1Pin, OUTPUT);
    pinMode(nozzle2Pin, OUTPUT);

    setNozzle(1, false);
    setNozzle(2, false);

    if (dht == nullptr)
    {
        sensorError = true;
        return;
    }

    dht->begin();
    readSensor();
}


// ================================================================
// SENSOR READING
// ================================================================

void GardenIrrigationDevice::readSensor()
{
    if (dht == nullptr)
    {
        sensorError = true;
        return;
    }

    float newHumidity = dht->readHumidity();
    float newTemperature = dht->readTemperature();

    if (isnan(newHumidity) || isnan(newTemperature))
    {
        sensorError = true;
        return;
    }

    humidity = newHumidity;
    temperature = newTemperature;

    sensorError = false;
}


// ================================================================
// IRRIGATION OUTPUT
// ================================================================

void GardenIrrigationDevice::setNozzle(
    uint8_t nozzle,
    bool state
)
{
    if (!hardwareConfigurationValid)
    {
        return;
    }

    uint8_t pin;

    if (nozzle == 1)
    {
        pin = nozzle1Pin;
        nozzle1On = state;

        if (state)
        {
            nozzle1StartedAt = millis();
        }
    }
    else if (nozzle == 2)
    {
        pin = nozzle2Pin;
        nozzle2On = state;

        if (state)
        {
            nozzle2StartedAt = millis();
        }
    }
    else
    {
        return;
    }

    bool gpioState;

    if (outputsActiveHigh)
    {
        gpioState = state ? HIGH : LOW;
    }
    else
    {
        gpioState = state ? LOW : HIGH;
    }

    digitalWrite(pin, gpioState);
}


// ================================================================
// AUTOMATIC WATERING TIMEOUT
// ================================================================

void GardenIrrigationDevice::updateAutomaticStop()
{
    unsigned long durationMs =
        static_cast<unsigned long>(
            wateringDurationSeconds
        ) * 1000UL;

    unsigned long now = millis();

    if (
        nozzle1On &&
        now - nozzle1StartedAt >= durationMs
    )
    {
        setNozzle(1, false);
    }

    if (
        nozzle2On &&
        now - nozzle2StartedAt >= durationMs
    )
    {
        setNozzle(2, false);
    }
}


// ================================================================
// HUMIDITY CONDITIONS
// ================================================================

bool GardenIrrigationDevice::isDry() const
{
    if (sensorError || isnan(humidity))
    {
        return false;
    }

    return humidity < dryThreshold;
}


bool GardenIrrigationDevice::isWet() const
{
    if (sensorError || isnan(humidity))
    {
        return false;
    }

    return humidity >= wetThreshold;
}


// ================================================================
// PERIODIC TASK
// ================================================================

void GardenIrrigationDevice::cyclic()
{
    readSensor();

    updateAutomaticStop();
}


// ================================================================
// DEVICE IDENTITY
// ================================================================

uint8_t GardenIrrigationDevice::getDeviceIdentifier()
{
    return deviceId;
}


uint8_t GardenIrrigationDevice::getDeviceType()
{
    return type_GARDEN_IRRIGATION;
}


uint16_t GardenIrrigationDevice::getExtendedMemoryLength()
{
    return 0;
}


// ================================================================
// DEVICE DESCRIPTION
// ================================================================

DeviceDescription GardenIrrigationDevice::getDeviceDescription()
{
    DeviceDescription description;

    description.deviceType = getDeviceType();

    description.deviceId = deviceId;

    description.roomId = roomId;

    // The main widget state represents whether either irrigation output is on.
    description.isEnabled = (nozzle1On || nozzle2On) ? 1 : 0;

    description.deviceName = deviceName;

    memset(
        description.customBytes,
        0,
        NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION
    );

    // ------------------------------------------------------------
    // STATE BYTE LAYOUT
    // ------------------------------------------------------------
    //
    // [0] sensor error
    //
    // [1..4] temperature float32
    //
    // [5..8] humidity float32
    //
    // [9] nozzle 1
    //
    // [10] nozzle 2
    //
    // [11] dry state
    //
    // [12] wet state
    //
    // ------------------------------------------------------------

    description.customBytes[0] =
        sensorError ? 1 : 0;

    memcpy(
        &description.customBytes[1],
        &temperature,
        sizeof(float)
    );

    memcpy(
        &description.customBytes[5],
        &humidity,
        sizeof(float)
    );

    description.customBytes[9] =
        nozzle1On ? 1 : 0;

    description.customBytes[10] =
        nozzle2On ? 1 : 0;

    description.customBytes[11] =
        isDry() ? 1 : 0;

    description.customBytes[12] =
        isWet() ? 1 : 0;

    return description;
}


// ================================================================
// SERVICE: NO PARAMETERS
// ================================================================

ServiceRequestErrorCode GardenIrrigationDevice::service(
    DeviceServicesType serviceType
)
{
    switch (serviceType)
    {
        default:
            return SERV_NOT_SUPPORTED;
    }
}


// ================================================================
// SERVICE: 5 BYTE PARAMETERS
// ================================================================
//
// parameters.a
//
// irrigation state:
// 0 = OFF
// 1 = ON
//
// ================================================================

ServiceRequestErrorCode GardenIrrigationDevice::service(
    DeviceServicesType serviceType,
    ServiceParameters_set1 parameters
)
{
    switch (serviceType)
    {
        case DEVSERVICE_STATE_SWITCH:
        {
            uint8_t state = parameters.a;

            if (state > 1 || !hardwareConfigurationValid)
            {
                return SERV_EXECUTION_FAILURE;
            }

            setNozzle(1, state != 0);
            setNozzle(2, state != 0);

            return SERV_SUCCESS;
        }

        default:
            return SERV_NOT_SUPPORTED;
    }
}


// ================================================================
// SERVICE: FLOAT PARAMETERS
// ================================================================

ServiceRequestErrorCode GardenIrrigationDevice::service(
    DeviceServicesType serviceType,
    ServiceParameters_set2 parameters
)
{
    return SERV_NOT_SUPPORTED;
}


// ================================================================
// SERVICE: RAW BUFFER
// ================================================================
//
// AdvancedControls payload:
//
// byte 0     watering duration LSB
// byte 1     watering duration MSB
//
// byte 2     dry threshold
//
// byte 3     wet threshold
//
// ================================================================

ServiceRequestErrorCode GardenIrrigationDevice::service(
    DeviceServicesType serviceType,
    ServiceParameters_set3 parameters
)
{
    constexpr uint16_t PAYLOAD_SIZE = 4;

    switch (serviceType)
    {
        // --------------------------------------------------------
        // ADVANCED CONTROLS GET
        // --------------------------------------------------------

        case DEVSERVICE_GET_ADVANCED_CONTROLS:
        {
            if (
                parameters.buff == nullptr ||
                parameters.size != PAYLOAD_SIZE ||
                parameters.direction != e_OUT_from_DEVICE
            )
            {
                return SERV_EXECUTION_FAILURE;
            }

            parameters.buff[0] =
                wateringDurationSeconds & 0xFF;

            parameters.buff[1] =
                wateringDurationSeconds >> 8;

            parameters.buff[2] =
                dryThreshold;

            parameters.buff[3] =
                wetThreshold;

            return SERV_SUCCESS;
        }


        // --------------------------------------------------------
        // ADVANCED CONTROLS SET
        // --------------------------------------------------------

        case DEVSERVICE_SET_ADVANCED_CONTROLS:
        {
            if (
                parameters.buff == nullptr ||
                parameters.size != PAYLOAD_SIZE ||
                parameters.direction != e_IN_to_DEVICE
            )
            {
                return SERV_EXECUTION_FAILURE;
            }

            uint16_t newDuration =
                static_cast<uint16_t>(
                    parameters.buff[0]
                ) |
                (
                    static_cast<uint16_t>(
                        parameters.buff[1]
                    ) << 8
                );

            uint8_t newDryThreshold =
                parameters.buff[2];

            uint8_t newWetThreshold =
                parameters.buff[3];


            // Validation.
            if (
                newDuration == 0 ||
                newDuration > 3600 ||
                newDryThreshold > 100 ||
                newWetThreshold > 100 ||
                newDryThreshold >= newWetThreshold
            )
            {
                return SERV_EXECUTION_FAILURE;
            }

            wateringDurationSeconds =
                newDuration;

            dryThreshold =
                newDryThreshold;

            wetThreshold =
                newWetThreshold;

            return SERV_SUCCESS;
        }


        // --------------------------------------------------------
        // ENABLING CONDITIONS
        // --------------------------------------------------------

        case DEVSERVICE_CHECK_ENABLING_CONDITION:
        {
            // Take a fresh reading before checking the condition.
            readSensor();

            if (sensorError)
            {
                return SERV_EXECUTION_FAILURE;
            }

            return GeneratedEnablingConditions::evaluateService(
                getDeviceDescription(),
                parameters
            );
        }


        default:
            return SERV_NOT_SUPPORTED;
    }
}