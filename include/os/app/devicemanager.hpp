#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <devices/OnOffDevice.hpp>
#include <devices/TestDeviceType.hpp>
#include <devices/LedWS1228b.hpp>
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
    static std::vector<LedWS1228bDeviceType> ledws2812bDevices;
    static std::vector<TempSensorDHT11DeviceType> tempSensorsDevices;

    static ConfigSlotsDataType pinConfigSlotsRamMirror;
    static ExtendedDataAllocator extDataAllocator;
    static void updateDeviceDescriptionSignal();

    static bool extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID);

    static uint16_t configCrcCalculation(uint8_t* data, uint16_t size);
    static DeviceConfigSlotType extractDeviceConfigFromString(String& confStr);

    static bool validateConfigurationData(ConfigSlotsDataType& data);

public:
    static void init();
    static void deinit();
    static void cyclic();
    
    static void setLocalConfigViaString(String& config);


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