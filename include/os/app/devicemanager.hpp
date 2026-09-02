#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <memory>
#include <vector>
#include <devices/device.hpp>
#include <os/datacontainer/datacontainertypes.hpp>
#include <os/tools/ExtendedDataAllocator.hpp>

/**
 * @file include/os/app/devicemanager.hpp
 * @brief Application-layer OS logic for the Home Control Station.
 */

/**
 * @class DeviceManager
 * @brief Owns the runtime device catalog and orchestrates initialization, configuration, and service calls.
 *
 * The concrete type set and factory wiring are supplied by the generated device registry. The manager
 * owns every instance through the Device interface, refreshes public descriptions for the UI, persists
 * the common NVM configuration, and routes service calls without depending on concrete device classes.
 */

class DeviceManager 
{
    /**
    * @brief Owns all device instances enabled by the generated registry.
    *
    * Registry metadata decides which instances are published to the common device collection.
     */
    static std::vector<std::unique_ptr<Device>> devices;

    /**
     * Last execution timestamp for each entry in devices. Both vectors are populated and cleared together.
     */
    static std::vector<uint32_t> lastDeviceCycleTimes;

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
    * Reconstructs a device through the generated factory registry from NVM slot content.
     * @param nvmData Stored data describing the device configuration.
     * @param configSlotID Slot index associated with the entity.
     * @return true when the instance could be created from the requested slot; false otherwise.
     */
    static bool extractDeviceInstanceBasedOnNvmData(
        DeviceConfigSlotType& nvmData,
        uint8_t configSlotID,
        bool* claimedPins,
        size_t claimedPinCount);


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
    * Runs devices according to their generated lifecycle intervals and publishes state once per second.
    * Fixed intervals use wrap-safe millisecond arithmetic and have the resolution of the manager task.
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
