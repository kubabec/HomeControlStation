#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <devices/OnOffDevice.hpp>
#include <devices/TestDeviceType.hpp>
#include <devices/LedStrip/LedWS1228b.hpp>
#include <devices/tempSensorDHT11.hpp>
#include <os/datacontainer/DataContainerTypes.hpp>
#include <os/tools/ExtendedDataAllocator.hpp>


typedef enum
{
    e_DEVICE_TYPE_FIRST = 43,
    e_ON_OFF_DEVICE = e_DEVICE_TYPE_FIRST,
    e_LED_STRIP,
    e_TEMP_SENSOR,
    e_DEVICE_TYPE_LAST = e_TEMP_SENSOR
}DeviceType;

class DeviceManager 
{
    /*TESTCODE*/
    static std::vector<Device*> devices;
    // static TestDeviceType testDev;
    /*TESTCODE*/
    static std::vector<OnOffDevice> vecOnOffDevices;
#ifdef LED_STRIP_SUPPORTED
    static std::vector<LedWS1228bDeviceType> ledws2812bDevices;
#endif
#ifdef TEMP_SENSOR_SUPPORTED
    static std::vector<TempSensorDHT11DeviceType> tempSensorsDevices;
#endif

    static ConfigSlotsDataType pinConfigSlotsRamMirror;
    static ExtendedDataAllocator extDataAllocator;
    static void updateDeviceDescriptionSignal();

    static bool extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID);


public:
    static void init();
    static void deinit();
    static void cyclic();
    
    static bool loadConfigFromFile(JsonDocument& doc);
    static bool setLocalSetupViaJson(String& json);
    static String getLocalSetupJson();

    static void getRtcTimeWrapper();

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