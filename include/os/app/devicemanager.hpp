#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <devices/OnOffDevice/OnOffDevice.hpp>
#include <devices/TestDeviceType/TestDeviceType.hpp>
#include <devices/LedWS1228bDeviceType/LedWS1228bDeviceType.hpp>
#include <devices/SegLedWS1228bDeviceType/SegLedWS1228bDeviceType.hpp>
#include <devices/TempSensorDHT11DeviceType/TempSensorDHT11DeviceType.hpp>
#include <devices/DistanceSensor/DistanceSensor.hpp>
#include <devices/HwButton/HwButton.hpp>
#include <os/datacontainer/DataContainerTypes.hpp>
#include <os/tools/ExtendedDataAllocator.hpp>

/**
 * @file include/os/app/devicemanager.hpp
 * @brief Application-layer OS logic for the Home Control Station.
 */

/**
 * @class DeviceManager
 * @brief Owns the runtime device catalog and orchestrates initialization, configuration, and service calls.
 *
 * This manager keeps the concrete device instances, refreshes their descriptions for the UI, persists
 * the NVM configuration, and routes service calls to the appropriate hardware class.
 */

class DeviceManager 
{
    /*TESTCODE*/
    /**
     * List of generic device pointers currently managed by the OS.
     */
    static std::vector<Device*> devices;
    // static TestDeviceType testDev;
    /*TESTCODE*/
    /**
     * Local instances of simple on/off devices managed by the Home Control Station.
     */
    static std::vector<OnOffDevice> vecOnOffDevices;
#ifdef LED_STRIP_SUPPORTED
    /**
     * WS2812B LED strip devices attached to the station.
     */
    static std::vector<LedWS1228bDeviceType> ledws2812bDevices;

    /**
     * Segmented LED strip devices requiring chunked control logic.
     */
    static std::vector<SegLedWS1228bDeviceType> segmentedWs2812bDevices;
#endif
#ifdef TEMP_SENSOR_SUPPORTED
    /**
     * Temperature sensor devices registered for periodic reporting.
     */
    static std::vector<TempSensorDHT11DeviceType> tempSensorsDevices;
#endif
#ifdef DISTANCE_SENSOR_SUPPORTED
    /**
     * Distance sensor devices used for presence or range detection.
     */
    static std::vector<DistanceSensor> distanceSensorsDevices;
#endif

    /**
     * Hardware buttons discovered and owned by the station.
     */
    static std::vector<HwButton> hardwareButtons;

    /**
     * RAM shadow of the pin configuration slots stored in NVM.
     */
    static ConfigSlotsDataType pinConfigSlotsRamMirror;

    /**
     * Allocator used for extended device memory and persistent payload storage.
     */
    static ExtendedDataAllocator extDataAllocator;

    /**
     * Updates the device description signal so the UI and remote-control subsystem see the current catalog.
     */
    static void updateDeviceDescriptionSignal();

    /**
     * Reconstructs a concrete device instance from the NVM configuration slot content.
     * @param nvmData Stored data describing the device configuration.
     * @param configSlotID Slot index associated with the entity.
     * @return true when the instance could be created from the requested slot; false otherwise.
     */
    static bool extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID);


public:
    /**
     * Initializes all configured device types and registers their runtime callbacks.
     */
    static void init();

    /**
     * Releases the managed device resources and closes optional runtime services.
     */
    static void deinit();

    /**
     * Runs the periodic device refresh cycle for state updates and config checks.
     */
    static void cyclic();

    /**
     * Flushes the in-memory configuration back to NVM when persistence is required.
     */
    static void flushNvmData();
    
    /**
     * Loads a device configuration from a JSON document.
     * @param doc JSON payload containing the desired local setup.
     * @return true when the configuration was accepted; false on invalid data.
     */
    static bool loadConfigFromFile(JsonDocument& doc);

    /**
     * Applies a local setup described by a JSON string to the current runtime configuration.
     * @param json Serialized device configuration.
     * @return true when the setup was applied successfully.
     */
    static bool setLocalSetupViaJson(String& json);

    /**
     * Serializes the current local setup into JSON form for persistence or UI export.
     * @return A JSON string containing the device configuration.
     */
    static String getLocalSetupJson();

    /**
     * Returns the current RTC time formatted in the application’s time structure.
     * @return Current device time represented by RtcTime.
     */
    static RtcTime getRtcTimeWrapper();

    /**
     * Handles the callback triggered after persistent data changes and refreshes related runtime state.
     */
    static void persistentDataChanged();

    /* TESTCODE */
    /**
     * Invokes a device service without additional parameters.
     * @param deviceId Device identifier being targeted.
     * @param serviceType Service requested from the device.
     * @return Service result code describing success or failure.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
    );

    /**
     * Invokes a device service using the set-1 service parameter payload.
     * @param deviceId Device identifier being targeted.
     * @param serviceType Service requested from the device.
     * @param param Service parameters in the single-value structure.
     * @return Service result code describing success or failure.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set1 param
    );

    /**
     * Invokes a device service using the set-2 service parameter payload.
     * @param deviceId Device identifier being targeted.
     * @param serviceType Service requested from the device.
     * @param param Service parameters in the dual-value structure.
     * @return Service result code describing success or failure.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set2 param
    );

    /**
     * Invokes a device service using the full set-3 parameter payload.
     * @param deviceId Device identifier being targeted.
     * @param serviceType Service requested from the device.
     * @param param Service parameters in the full structure.
     * @return Service result code describing success or failure.
     */
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set3 param
    );
    /* TESTCODE */
    

};


#endif
