#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H
#include "devices/Device.hpp"
#include "Arduino.h"

class DistanceSensor : public Device {

    int pinNumber;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    
    public:
    DistanceSensor();
    DistanceSensor(DeviceConfigSlotType nvmData);

    virtual void init();//funkcje ktore nazucaja potomka koniecznosc ich implementacji
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