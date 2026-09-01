#ifndef ADVANCED_CONTROLS_LOADER_H
#define ADVANCED_CONTROLS_LOADER_H

#include <Arduino.h>
#include <devices/device.hpp>
#include <os/datacontainer/DataContainer.hpp>

/**
 * @file AdvancedControlsLoader.hpp
 * @brief Device-independent transport for advanced-controls templates and payloads.
 */

/**
 * @class AdvancedControlsLoader
 * @brief Loads a device payload through the common advanced-controls service.
 *
 * Device-specific presentation is supplied by a generated registry of HTML patterns referenced
 * from device JSON descriptions. The platform neither includes concrete device classes nor
 * interprets their payload layout.
 */
class AdvancedControlsLoader
{
    static DeviceDescription currentlyRequestedDeviceDescription;
    static uint8_t* currentAdvancedControls;
    static String currentResponse;

    static uint16_t getControlsSizeBasedOnDevType(uint8_t deviceType);
    static uint8_t* allocateMemoryForControlsBasedOnDeviceType(uint8_t deviceType);
    static void prepareResponse();
    static void appendJsonString(String& destination, const char* value);

public:
    /**
     * Loads advanced-controls bytes through `DEVSERVICE_GET_ADVANCED_CONTROLS`.
     * @param deviceIdentifier Runtime ID of the requested device.
     * @return Service progress or result.
     */
    static ServiceRequestErrorCode loadAdvancedControls(uint16_t deviceIdentifier);

    /**
     * Returns the generated template and opaque payload as a JSON response.
     * @return JSON consumed by the generic browser-side advanced-controls host.
     */
    static String getOutput();
};

#endif
