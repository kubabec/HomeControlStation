#ifndef ADVANCED_CONTROLS_LOADER_H
#define ADVANCED_CONTROLS_LOADER_H
#include <Arduino.h>
#include <map>
#include <devices/device.hpp>
#include <os/datacontainer/DataContainer.hpp>
/**
 * @class AdvancedControlsLoader
 * @brief Loads advanced-control payloads and delegates rendering through generated type dispatch.
 *
 * Payload sizes and renderer selection are generated from device JSON metadata. Specialized renderer
 * implementations remain responsible for interpreting device-owned payload formats.
 */


class AdvancedControlsLoader{
    static DeviceDescription currentlyRequestedDeviceDescription;
    static uint8_t* currentAdvancedControls;

    static String currentRequestJS;

    static uint16_t getControlsSizeBasedOnDevType(uint8_t deviceType);
    static uint8_t* allocateMemoryForControlsBasedOnDeviceType(uint8_t deviceType);
    static void prepareJsStringWithAdvancedControls();

    static String createJsForOnOff();
    static String createJsForLedStrip();

    public :

    static ServiceRequestErrorCode loadAdvancedControlsToJavaScript(
        uint16_t deviceIdentifier
    );


    static String getOutpuJavaScript();
};


#endif