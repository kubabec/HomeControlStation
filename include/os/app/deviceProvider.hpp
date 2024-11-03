#ifndef DEVICE_PROVIDER_H
#define DEVICE_PROVIDER_H
#include <Arduino.h>
#include <os/datacontainer/DataContainer.hpp>
#include <map>
#include <any>
#include <functional>

/* typ opisujacy czy dane urzadzenie jest local czy remote i jakie ma oryginalne ID */
struct DeviceTranslationDetails{
    uint8_t originalID = 255;
    bool isLocal = true;
};

class DeviceProvider 
{
    static std::function<bool(uint8_t, bool)> deviceManager_DeviceEnable;
    static std::function<bool(uint8_t, uint8_t)> deviceManager_BrightnessChange;
    static std::function<bool(uint8_t, bool)> rcServer_DeviceEnable;
    static std::function<bool(uint8_t, uint8_t)> rcServer_BrightnessChange;
    static std::function<bool(SystemResponse&)> requestResponse;


    static bool isRCServer;

    static std::map<uint8_t, DeviceTranslationDetails> uniqueDeviceIdToNormalDeviceIdMap;
    
    static void updateDeviceDescriptionSignal();
    static DeviceTranslationDetails getOriginalIdFromUnique(uint8_t uniqueId);
    //static uint8_t findUniqueIdByOriginalId(uint8_t originalId);

    static void initLocalDevicesSetup();
    static void initRemoteDevicesSetup();

    


public:
    static void init();
    static void deinit();
    static void cyclic();

    static bool deviceEnable(uint8_t deviceId, bool state);
    static bool deviceBrightnessChange(uint8_t deviceId, uint8_t brightnessLevel);

    static void deviceReset();

    static void updateDeviceDescriptionSignal_onChange(std::any signal);

    static void printIdMap();

    static bool receiveSystemRequest(SystemRequest& request);

        
};


#endif