#ifndef DEVICE_CONTROL_API_H
#define DEVICE_CONTROL_API_H

#include <functional>

/**
 * @file include/os/datacontainer/DeviceControlApi.hpp
 * @brief Data container and persistence definitions for the Home Control Station operating system.
 */


typedef struct{ 
    /*
        uint8_t device identifier - ID of the device
        bool state                - Requested state Enable/Disable

        @return bool              - operation success
    */
    std::function<bool(uint8_t, bool)> setDeviceState; 

    /*
        uint8_t device identifier  - ID of the device
        uint8_t brigthness level   - 0 - 100

        @return bool               - operation success
    */
    std::function<bool(uint8_t, uint8_t)> changeBrightness; 

}DeviceControlFunctionSet;


#endif