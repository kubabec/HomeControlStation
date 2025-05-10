#ifndef TEMP_SENSOR_DHT11_TYPE_H
#define TEMP_SENSOR_DHT11_TYPE_H
#include <SystemDefinition.hpp>
#ifdef TEMP_SENSOR_SUPPORTED


#include "devices/Device.hpp"
#include "Arduino.h"
#include <DHT.h>

class TempSensorDHT11DeviceType : public Device
{

    bool isOn = true; // stan urzadzenia
    int pinNumber;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    float currentTemp = 255.f;
    uint8_t currentHumid = 255;

    long lastDataUpdateTime = 0;

    DHT *dht = nullptr;

public:
    TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData);

    virtual void init();
    virtual void cyclic();
    virtual uint8_t getDeviceIdentifier();
    virtual uint8_t getDeviceType();
    virtual DeviceDescription getDeviceDescription();
    virtual uint16_t getExtendedMemoryLength();

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);
};

#endif

#endif