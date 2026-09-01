/**
 * @file DistanceSensor.hpp
 * @brief Distance sensor device abstraction for proximity-based measurements.
 */
#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H
#include "devices/Device.hpp"
#include "Arduino.h"

/**
 * @class DistanceSensor
 * @brief Device implementing a proximity sensor using the shared device interface.
 */
class DistanceSensor : public Device {

    /** GPIO pin used by the distance sensor. */
    int pinNumber;
    /** Device instance identifier. */
    uint8_t deviceId;
    /** User-visible name of the sensor. */
    String deviceName;
    /** Room association of the sensor. */
    uint8_t roomId;
    
public:
    /**
     * @brief Creates a default distance sensor instance.
     */
    DistanceSensor();
    /**
     * @brief Restores the sensor from the saved NVM configuration block.
     * @param nvmData Configuration payload persisted in non-volatile memory.
     */
    DistanceSensor(DeviceConfigSlotType nvmData);

    /**
     * @brief Initializes the sensor hardware and its internal state.
     */
    virtual void init();
    /**
     * @brief Runs the periodic measurement loop of the sensor.
     */
    virtual void cyclic();
    /**
     * @brief Returns the sensor instance index.
     * @return Device identifier value.
     */
    virtual uint8_t getDeviceIdentifier();
    /**
     * @brief Returns the distance-sensor type identifier.
     * @return Device class type code.
     */
    virtual uint8_t getDeviceType();
    /**
     * @brief Produces the runtime metadata used by the OS.
     * @return DeviceDescription of the sensor.
     */
    virtual DeviceDescription getDeviceDescription();
    /**
     * @brief Returns the memory footprint used by extended device metadata.
     * @return Custom memory length in bytes.
     */
    virtual uint16_t getExtendedMemoryLength();

    /**
     * @brief Handles a service call without extra payload.
     * @param serviceType Requested service command.
     * @return Status returned by the operation.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    /**
     * @brief Handles a service call with a numeric parameter set.
     * @param serviceType Requested service command.
     * @param param Parameter payload.
     * @return Status returned by the operation.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    /**
     * @brief Handles a service call with floating-point parameter data.
     * @param serviceType Requested service command.
     * @param param Parameter payload.
     * @return Status returned by the operation.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    /**
     * @brief Handles a service call with buffered parameter data.
     * @param serviceType Requested service command.
     * @param param Parameter payload.
     * @return Status returned by the operation.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);


};



#endif
