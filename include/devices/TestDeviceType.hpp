#ifndef TEST_DEVICE_TYPE_H
#define TEST_DEVICE_TYPE_H
#include "devices/Device.hpp"
#include "Arduino.h"

class TestDeviceType : public Device {


    public:
    virtual void init();//funkcje ktore nazucaja potomka koniecznosc ich implementacji
    virtual void cyclic();
    virtual uint8_t getDeviceIdentifier();
    virtual uint8_t getDeviceType();

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);


};



#endif