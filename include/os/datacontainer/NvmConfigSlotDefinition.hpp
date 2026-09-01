/**
 * @file include/os/datacontainer/NvmConfigSlotDefinition.hpp
 * @brief Operating-system runtime and networking support for the Home Control Station.
 */

#ifndef CONFIG_SLOT_DEFINITION_H
#define CONFIG_SLOT_DEFINITION_H
#include <Arduino.h>
#include "os/Logger.hpp"
#include "generated/GeneratedDeviceTypes.hpp"

/* Description of single configuration NVM slot */
/**
 * @class DeviceConfigSlotType
 * @brief Represents the Device Config Slot Type data structure.
 */
struct DeviceConfigSlotType
{
    bool isActive = false;            /* 1 byte */
    char deviceName[25] = {'\0'};   /* 25 bytes */
    uint8_t deviceType = 255;       /* 1 byte */
    uint8_t pinNumber = 255;        /* 1 byte */
    uint8_t deviceId = 255;         /* 1 byte */
    uint8_t roomId = 255;           /* 1 byte */
    uint8_t customBytes[20] = {0x00};          /* 20 bytes */

    DeviceConfigSlotType(){
        memset(deviceName, (char)'\0', 25);
        memset(customBytes, 0x00, 20);
    }

    void print()
    {
        if(!isActive){
            Logger::log("Config SLOT is not active");
            return;
        }

        String logMsg = "--> Config SLOT <-- | ";
        logMsg += "isActive: " + String(isActive ? "YES" : "NO") + " | ";
        logMsg += "Device Type: ";
        logMsg += deviceType == 255 ? "Not configured" : GeneratedDeviceTypes::nameOf(deviceType);
        logMsg += " | Device ID: " + String(deviceId);
        logMsg += " | Name: " + String(deviceName);
        logMsg += " | PIN: " + String((int)pinNumber);
        logMsg += " | Room: " + String((int)roomId);
        logMsg += " | Extra data: ";
        for(int i = 0 ; i < 20; i++)
        {
            logMsg += String((int)customBytes[i]) + " ";
        }
        Logger::log(logMsg);
    }

    bool isValid()
    {
        return GeneratedDeviceTypes::isKnown(deviceType);
    }

    static uint8_t getSize()
    {
        return sizeof(isActive) + 25 + sizeof(deviceType) + sizeof(pinNumber) + sizeof(deviceId) + sizeof(roomId) + 20;
    }

};

#endif