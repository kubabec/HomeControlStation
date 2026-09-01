/**
 * @file device.hpp
 * @brief Shared abstraction layer for device metadata, service calls, and runtime state.
 */
#ifndef DEVICE_H
#define DEVICE_H
#include "Arduino.h"
#include "os/datacontainer/NvmConfigSlotDefinition.hpp"
#include "AdvancedControls.hpp"
#include "os/Logger.hpp"
#include "generated/GeneratedDeviceTypes.hpp"

#define NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION 50
#define DEVICE_NAME_MAX_LENGHT 25

#define SERVICE_OVERLOADING_FUNCTION_INDEX 1
#define SERVICE_NAME_INDEX 0

/**
 * @brief Enumerates the supported service overload variants.
 */
enum ServiceOverloadingFunction
{
    serviceCall_NoParams,
    serviceCall_1,
    serviceCall_2,
    serviceCall_3,
    serviceCall_INVALID
};

/**
 * @brief Standard return codes used by device service operations.
 */
typedef enum
{
    SERV_SUCCESS,
    SERV_EXECUTION_FAILURE,
    SERV_GENERAL_FAILURE,
    SERV_BUSY,
    SERV_PENDING,
    SERV_NOT_SUPPORTED
} ServiceRequestErrorCode;

/**
 * @brief High-level service commands that a device may support.
 */
typedef enum
{
    DEVSERVICE_ENABLE,
    DEVSERVICE_DISABLE,
    DEVSERVICE_SET_DATA,
    DEVSERVICE_GET_DATA,
    DEVSERVICE_STATE_SWITCH,
    DEVSERVICE_LED_STRIP_SAVE_CONTENT,
    DEVSERVICE_LED_STRIP_SWITCH_CONTENT,
    DEVSERVICE_BRIGHTNESS_CHANGE,
    DEVSERVICE_SET_EXT_MEMORY_PTR,
    DEVSERVICE_GET_ADVANCED_CONTROLS,
    DEVSERVICE_GET_DETAILED_COLORS,
    DEVSERVICE_SET_DETAILED_COLORS,
    DEVSERVICE_ROOM_STATE_CHANGE,
    DEVSERVICE_SEGMENT_STATE_SWITCH,
    DEVSERVICE_LIVE_ANIMATION,
    /** Common opaque payload write used by every generated advanced-controls widget. */
    DEVSERVICE_SET_ADVANCED_CONTROLS,
    DEVSERVICE_INVALID
} DeviceServicesType;

/**
 * @brief Represents the time descriptor used by sensor and device records.
 */
struct RtcTime
{
    /** Seconds field. */
    int sec = 0;
    /** Minutes field. */
    int min = 0;
    /** Hours field. */
    int hour = 0;
    /** Day of month. */
    int mday = 0;
    /** Month number. */
    int mon = 0;
    /** Current year. */
    int year = 0;
    /** Day of week. */
    int wday = 0;
    /** Day of year. */
    int yday = 0;
    /** Daylight-saving flag. */
    int isdst = 0;

    /**
     * @brief Serializes the timestamp into a readable string.
     * @return String formatted as YYYY.MM.DD HH:MM:SS.
     */
    String toString()
    {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "%04d.%02d.%02d %02d:%02d:%02d",
                 year, mon, mday,
                 hour, min, sec);
        return String(buffer);
    }
};

/**
 * @brief Lightweight parameter block used for single-value service calls.
 */
typedef struct
{
    /** First numeric value. */
    uint8_t a = 0xff;
    /** Second numeric value. */
    uint8_t b = 0xff;
    /** Third numeric value. */
    uint8_t c = 0xff;
    /** Fourth numeric value. */
    uint8_t d = 0xff;
    /** Fifth numeric value. */
    uint8_t e = 0xff;
} ServiceParameters_set1;

/**
 * @brief Parameter block carrying floating-point values for service calls.
 */
typedef struct
{
    /** First floating-point value. */
    float a = 0xff;
    /** Second floating-point value. */
    float b = 0xff;
    /** Third floating-point value. */
    float c = 0xff;
    /** Fourth floating-point value. */
    float d = 0xff;
    /** Fifth floating-point value. */
    float e = 0xff;
} ServiceParameters_set2;

typedef enum
{
    e_OUT_from_DEVICE = 15,
    e_IN_to_DEVICE,
    e_UNKNOWN_PARAM_DIRECTION
} ServiceDirectionType;

/**
 * @brief Structured payload used for raw buffer transfers or extended memory access.
 */
typedef struct
{
    /** Pointer to the attached byte buffer. */
    uint8_t *buff = 0;
    /** Number of bytes contained in the payload. */
    uint16_t size = 0xff;
    /** Additional metadata supplied by the caller. */
    uint16_t additionalParam = 0xFF;
    /** Direction of the transfer between device and host. */
    uint8_t direction = (uint8_t)e_UNKNOWN_PARAM_DIRECTION;

    /**
     * @brief Logs all fields stored in the buffer parameter object.
     */
    void print()
    {
        Logger::log("ServiceParameters_set3");
        Logger::log("buff: " + String((int)buff));
        Logger::log("size: " + String((int)size));
        Logger::log("additionalParam: " + String((int)additionalParam));
        switch (direction)
        {
        case e_UNKNOWN_PARAM_DIRECTION:
            Logger::log("direction : e_UNKNOWN_PARAM_DIRECTION");
            break;
        case e_OUT_from_DEVICE:
            Logger::log("direction : e_OUT_from_DEVICE");
            break;
        case e_IN_to_DEVICE:
            Logger::log("direction : e_IN_to_DEVICE");
            break;
        }
    }
} ServiceParameters_set3;

/**
 * @brief Runtime representation of a device state and metadata snapshot.
 *
 * This structure is used to describe a device over the network and to persist
 * configuration across reboots.
 */
struct DeviceDescription
{
    /** Hardware type identifier. */
    uint8_t deviceType = 255;
    /** Unique MAC address of the device. */
    uint64_t macAddress = 0LL;
    /** Runtime device instance number. */
    uint8_t deviceId = -1;
    /** Room or zone index associated with the device. */
    uint8_t roomId = 255;
    /** Indicates whether the device is active. */
    uint8_t isEnabled;
    /** Free-form bytes reserved for custom metadata. */
    uint8_t customBytes[NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION] = {0x00};

    /** Human-readable device name. */
    String deviceName;

    /**
     * @brief Initializes the custom metadata area with zeros.
     */
    DeviceDescription()
    {
        memset(customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    }

    /**
     * @brief Logs the full device description to the system logger.
     */
    void print()
    {
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X",
                 (uint8_t)(macAddress >> 56), (uint8_t)(macAddress >> 48),
                 (uint8_t)(macAddress >> 40), (uint8_t)(macAddress >> 32),
                 (uint8_t)(macAddress >> 24), (uint8_t)(macAddress >> 16),
                 (uint8_t)(macAddress >> 8), (uint8_t)(macAddress));
        String devTypeStr = GeneratedDeviceTypes::nameOf(deviceType);
        String out = "deviceType: {" + devTypeStr + "} | ";
        out += "macAddress: " + String(macStr) + " | ";
        out += "deviceId: " + String(deviceId) + " | ";
        out += "isEnabled: " + String(isEnabled) + " | ";
        out += "deviceName: " + deviceName + " | ";
        out += "customBytes: ";
        for (int i = 0; i < NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION; i++)
        {
            out += String(customBytes[i]) + " ";
        }
        Logger::log(out);
    }

    /**
     * @brief Returns the serialized size of the device description payload.
     * @return Byte length of the persisted description block.
     */
    uint16_t getSize()
    {
        return (sizeof(deviceType) + sizeof(macAddress) + sizeof(deviceId) + sizeof(roomId) + sizeof(isEnabled) + DEVICE_NAME_MAX_LENGHT + NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    };

    /**
     * @brief Serializes the description to a byte buffer.
     * @param buffer Destination memory block.
     * @param size Expected size in bytes.
     * @return True when the payload was encoded successfully.
     */
    bool toByteArray(uint8_t *buffer, uint16_t size)
    {
        uint16_t offset = 0;
        if (buffer != nullptr && size == getSize())
        {
            memcpy(&buffer[offset], &deviceType, sizeof(deviceType));
            offset += sizeof(deviceType);

            memcpy(&buffer[offset], &macAddress, sizeof(macAddress));
            offset += sizeof(macAddress);

            memcpy(&buffer[offset], &deviceId, sizeof(deviceId));
            offset += sizeof(deviceId);

            memcpy(&buffer[offset], &roomId, sizeof(roomId));
            offset += sizeof(roomId);

            memcpy(&buffer[offset], &isEnabled, sizeof(isEnabled));
            offset += sizeof(isEnabled);

            /* prepare empty space string value */
            memset(&buffer[offset], '\0', DEVICE_NAME_MAX_LENGHT);
            if (deviceName.length() < DEVICE_NAME_MAX_LENGHT)
            {
                deviceName.getBytes(&buffer[offset], DEVICE_NAME_MAX_LENGHT);
            }
            else
            {
                return false;
            }
            offset += DEVICE_NAME_MAX_LENGHT;

            memcpy(&buffer[offset], customBytes, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

            return true;
        }
        return false;
    }

    /**
     * @brief Restores the description from a serialized byte array.
     * @param buffer Source memory block.
     * @param size Expected payload length.
     * @return True when the data was decoded successfully.
     */
    bool fromByteArray(uint8_t *buffer, uint16_t size)
    {
        uint16_t offset = 0;
        if (buffer != nullptr && size == getSize())
        {
            memcpy(&deviceType, &buffer[offset], sizeof(deviceType));
            offset += sizeof(deviceType);

            memcpy(&macAddress, &buffer[offset], sizeof(macAddress));
            offset += sizeof(macAddress);

            memcpy(&deviceId, &buffer[offset], sizeof(deviceId));
            offset += sizeof(deviceId);

            memcpy(&roomId, &buffer[offset], sizeof(roomId));
            offset += sizeof(roomId);

            memcpy(&isEnabled, &buffer[offset], sizeof(isEnabled));
            offset += sizeof(isEnabled);

            /* prepare empty space string value */
            deviceName = String((char *)&buffer[offset]);
            if (deviceName.length() > DEVICE_NAME_MAX_LENGHT)
            {
                return false;
            }
            offset += DEVICE_NAME_MAX_LENGHT;

            memcpy(customBytes, &buffer[offset], NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

            return true;
        }
        else
        {
            Logger::log("Wrong buffer length");
        }
        return false;
    }
};

/**
 * @class Device
 * @brief Abstract base interface implemented by all runtime devices.
 *
 * Each device provides lifecycle hooks, a description payload, and service
 * methods that allow the operating system to control, inspect, and persist its
 * state.
 */
class Device
{
private:
    /** Internal device identifier for the default runtime mapping. */
    uint8_t DeviceIdentifier = 0xFF;

public:
    /** Allows device instances to be owned and destroyed through the base interface. */
    virtual ~Device() = default;

    /**
     * @brief Initializes the device and prepares its hardware resources.
     */
    virtual void init() = 0;
    /**
     * @brief Executes the recurring processing loop of the device.
     */
    virtual void cyclic() = 0;
    /**
     * @brief Returns the serialized metadata describing the device.
     * @return DeviceDescription snapshot with system metadata.
     */
    virtual DeviceDescription getDeviceDescription() = 0;
    /**
     * @brief Returns the unique runtime identifier for the current device.
     * @return Device index or identifier value.
     */
    virtual uint8_t getDeviceIdentifier() = 0;
    /**
     * @brief Returns the device category type.
     * @return Type enumeration value defined by DevType.
     */
    virtual uint8_t getDeviceType() = 0;
    /**
     * @brief Reports the length of the extended memory block associated with the device.
     * @return Number of bytes reserved for custom state.
     */
    virtual uint16_t getExtendedMemoryLength() = 0;

    /**
     * @brief Executes a service request without additional parameters.
     * @param serviceType Requested service command.
     * @return Result code describing execution status.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType) = 0;
    /**
     * @brief Executes a service request accepting an 8-bit payload set.
     * @param serviceType Requested service command.
     * @param param Parameter block containing numeric values.
     * @return Result code describing execution status.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param) = 0;
    /**
     * @brief Executes a service request accepting floating-point payloads.
     * @param serviceType Requested service command.
     * @param param Parameter block containing floating-point data.
     * @return Result code describing execution status.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param) = 0;
    /**
     * @brief Executes a service request accepting buffered payload data.
     * @param serviceType Requested service command.
     * @param param Parameter block containing raw memory and transfer metadata.
     * @return Result code describing execution status.
     */
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param) = 0;
};

#endif