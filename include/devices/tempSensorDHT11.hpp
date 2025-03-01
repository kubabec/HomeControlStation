#ifndef TEMP_SENSOR_DHT11_TYPE_H
#define TEMP_SENSOR_DHT11_TYPE_H
#include "devices/Device.hpp"
#include "Arduino.h"


class TempSensorDHT11DeviceType : public Device {

    bool isOn = true; //stan urzadzenia
    int pinNumber;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    float currentTemp = -10.f;
    uint8_t currentHumid = 0.f;

    long lastDataUpdateTime = 0;
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