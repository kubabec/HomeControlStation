/**
 * @file TempSensorDHT11DeviceType.hpp
 * @brief Temperature and humidity sensor implementation using a DHT22 device.
 */
#ifndef TEMP_SENSOR_DHT11_TYPE_H
#define TEMP_SENSOR_DHT11_TYPE_H
#include <SystemDefinition.hpp>
#ifdef TEMP_SENSOR_SUPPORTED


#include "devices/device.hpp"
#include "Arduino.h"
#include <DHT.h>

/**
 * @class TempSensorDHT11DeviceType
 * @brief Reads and stores temperature and humidity values for the control station.
 */
class TempSensorDHT11DeviceType : public Device
{
    static constexpr uint32_t HISTORY_MAGIC = 0x54485331;
    static constexpr uint8_t HISTORY_VERSION = 1;
    static constexpr uint8_t HISTORY_RECORD_COUNT = 14;
    static constexpr uint16_t HISTORY_HEADER_SIZE = 16;
    static constexpr uint16_t HISTORY_RECORD_SIZE = 24;
    static constexpr uint16_t HISTORY_SIZE = HISTORY_HEADER_SIZE + HISTORY_RECORD_COUNT * HISTORY_RECORD_SIZE;

    /** Indicates whether the sensor is currently enabled. */
    bool isOn = true;
    /** GPIO pin connected to the DHT sensor. */
    int pinNumber;
    /** System ID of the device. */
    uint8_t deviceId;
    /** Human-readable name used in the UI. */
    String deviceName;
    /** Room index of the sensor. */
    uint8_t roomId;
    /** Latest measured temperature. */
    float currentTemp = 255.f;
    /** Latest measured humidity percentage. */
    uint8_t currentHumid = 255;
    /** Error status for the temperature/humidity sensor. */
    uint8_t temHumSensError = 0;
    /** Previous temperature value retained for comparisons. */
    float lastTemp = 0;
    /** Persistent ring buffer supplied by ExtendedMemoryManager. */
    uint8_t *historyData = nullptr;

    /** Concrete DHT sensor object used for reading the environment. */
    DHT *dht = nullptr;
    /** Callback used to obtain the current system time. */
    std::function<RtcTime(void)> getTime;
    /** Requests a deferred NVM flush after a half-day statistics period closes. */
    std::function<void(void)> reportNvmDataChanged;
    /** Reads raw temperature and humidity from the DHT sensor. */
    virtual void temHumReading();
    /** Adds the current reading to the active half-day aggregate. */
    void recordStatistics();
    /** Initializes an empty, versioned history buffer. */
    void initializeHistory();
    /** Returns the offset of a ring-buffer record. */
    uint16_t recordOffset(uint8_t index) const;
    
public:
    /**
        * @brief Constructs the DHT22-based sensor from persisted configuration.
     * @param nvmData Persisted device configuration.
     * @param getTimeCallback Callback returning the current RTC time.
     */
    TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData,
                              std::function<RtcTime(void)> getTimeCallback,
                              std::function<void(void)> reportNvmDataChangedCallback = {});

    /**
        * @brief Performs device initialization.
        *
        * The current implementation performs no additional initialization because
        * the DHT sensor is initialized by the constructor.
     */
    virtual void init();
    /**
     * @brief Executes the periodic sensor acquisition loop.
     */
    virtual void cyclic();
    /**
     * @brief Returns the runtime sensor identifier.
     * @return Device identifier value.
     */
    virtual uint8_t getDeviceIdentifier();
    /**
     * @brief Returns the environment sensor type.
        * @return Device type code for the temperature sensor.
     */
    virtual uint8_t getDeviceType();
    /**
     * @brief Produces the sensor metadata used by the runtime.
     * @return Device description snapshot.
     */
    virtual DeviceDescription getDeviceDescription();
    /**
     * @brief Reports the size of persistent extended memory used by the device.
     * @return Number of bytes reserved by the sensor state.
     */
    virtual uint16_t getExtendedMemoryLength();

    /**
     * @brief Executes a general service request for the sensor.
     * @param serviceType Requested command.
     * @return Result of the service execution.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    /**
     * @brief Executes a single-value service request for the sensor.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Result of the service execution.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    /**
     * @brief Executes a floating-point request for the sensor.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Result of the service execution.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    /**
     * @brief Executes a raw-buffer service request for the sensor.
     * @param serviceType Requested command.
     * @param param Parameter payload.
     * @return Result of the service execution.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);
};

#endif

#endif
