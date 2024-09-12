#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <devices/OnOffDevice.hpp>
#include <os/datacontainer/DataContainerTypes.hpp>


typedef enum
{
    e_DEVICE_TYPE_FIRST = 43,
    e_ON_OFF_DEVICE = e_DEVICE_TYPE_FIRST,
    e_LED_STRIP,
    e_DEVICE_TYPE_LAST = e_LED_STRIP
}DeviceType;

typedef struct
{
    char deviceName[25] = {'\0'};   /* 25 bytes */
    uint8_t deviceType = 255;       /* 1 byte */
    uint8_t pinNumber = 255;        /* 1 byte */
    uint8_t deviceId = 255;         /* 1 byte */
    uint8_t roomId = 255;           /* 1 byte */
    bool brightnessSupport = false; /* 1 byte */
    uint8_t lastBrightness = 0;     /* 1 byte */
    uint8_t lastState = 0;          /* 1 byte */
                                    /* ---------- 32 bytes */

    uint8_t customBytes[10] = {0};  /* 10 bytes */
                                    /* ---------- 42 bytes */
    uint8_t reserved[8];           /*  8 bytes to align with PERSISTENT_DATABLOCK_SIZE */

    static uint16_t getSize()
    {
        return 25 + 
            sizeof(deviceType) +
            sizeof(pinNumber) + 
            sizeof(deviceId) + 
            sizeof(roomId) + 
            sizeof(brightnessSupport) + 
            sizeof(lastBrightness) +
            sizeof(lastState) +
            10 + /*custom bytes */
            8; // reserved;
    }

    bool isValid()
    {
        return (pinNumber != 255 && deviceId != 255);
    }
}OnOffConfigDatablock;

class DeviceManager 
{

    static std::vector<OnOffDevice> vecOnOffDevices;
    static ConfigSlotsDataType pinConfigSlotsRamMirror;
    static void updateDeviceDescriptionSignal();

    static bool extractDeviceInstanceBasedOnNvmData(OnOffConfigDatablock& nvmData, uint8_t configSlotID);
public:
    static void init();
    static void cyclic();
    static bool deviceEnable(uint8_t deviceId, bool state);
    static bool deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel);
    

};


#endif