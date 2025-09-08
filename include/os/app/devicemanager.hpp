#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H
#include <Arduino.h>
#include <devices/OnOffDevice.hpp>
#include <devices/TestDeviceType.hpp>
#include <devices/LedStrip/LedWS1228b.hpp>
#include <devices/LedStrip/Segmented_LedWS1228b.hpp>
#include <devices/tempSensorDHT11.hpp>
#include <devices/distanceSensor.hpp>
#include <devices/CustomHardware/HwButton.hpp>
#include <os/datacontainer/DataContainerTypes.hpp>
#include <os/tools/ExtendedDataAllocator.hpp>




class DeviceManager 
{
    /*TESTCODE*/
    static std::vector<Device*> devices;
    // static TestDeviceType testDev;
    /*TESTCODE*/
    static std::vector<OnOffDevice> vecOnOffDevices;
#ifdef LED_STRIP_SUPPORTED
    static std::vector<LedWS1228bDeviceType> ledws2812bDevices;
    static std::vector<SegLedWS1228bDeviceType> segmentedWs2812bDevices;
#endif
#ifdef TEMP_SENSOR_SUPPORTED
    static std::vector<TempSensorDHT11DeviceType> tempSensorsDevices;
#endif
#ifdef DISTANCE_SENSOR_SUPPORTED
    static std::vector<DistanceSensor> distanceSensorsDevices;
#endif

    // Vector of hardware buttons, if any exists
    static std::vector<HwButton> hardwareButtons;

    static ConfigSlotsDataType pinConfigSlotsRamMirror;
    static ExtendedDataAllocator extDataAllocator;
    static void updateDeviceDescriptionSignal();

    static bool extractDeviceInstanceBasedOnNvmData(DeviceConfigSlotType& nvmData, uint8_t configSlotID);


public:
    static void init();
    static void deinit();
    static void cyclic();
    static void flushNvmData();
    
    static bool loadConfigFromFile(JsonDocument& doc);
    static bool setLocalSetupViaJson(String& json);
    static String getLocalSetupJson();

    static RtcTime getRtcTimeWrapper();
    static void persistentDataChanged();

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