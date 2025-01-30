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
    static std::function<bool(RcResponse&)> sendResponse;
    static std::function<bool(RcResponseLong&)> requestResponseLong;


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

    static void deviceReset();

    static void updateDeviceDescriptionSignal_onChange(std::any signal);

    static void printIdMap();

    static bool receiveRequest(RcRequest& request);
    

    /* TESTCODE */
    static ServiceRequestErrorCode service(
        uint8_t deviceId, 
        DeviceServicesType serviceType
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set1 param
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set2 param
    );
    static ServiceRequestErrorCode service(
        uint8_t deviceId,
        DeviceServicesType serviceType,
        ServiceParameters_set3 param
    );
    /* TESTCODE */
        
};


#endif