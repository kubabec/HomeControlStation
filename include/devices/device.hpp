#ifndef DEVICE_H
#define DEVICE_H
#include "Arduino.h"

typedef enum
{
    SERV_SUCCESS,
    SERV_EXECUTION_FAILURE,
    SERV_GENERAL_FAILURE,
    SERV_NOT_SUPPORTED
}ServiceRequestErrorCode;

typedef enum
{
    DEVSERVICE_ENABLE,
    DEVSERVICE_DISABLE,
    DEVSERVICE_SET_DATA,
    DEVSERVICE_GET_DATA,
    DEVSERVICE_STATE_SWITCH,
    DEVSERVICE_BRIGHTNESS_CHANGE
}DeviceServicesType;

typedef struct
{
    uint8_t a = 0xff;
    uint8_t b = 0xff;
    uint8_t c = 0xff;
    uint8_t d = 0xff;
    uint8_t e = 0xff;
}ServiceParameters_set1;

typedef struct
{
    float a = 0xff;
    float b = 0xff;
    float c = 0xff;
    float d = 0xff;
    float e = 0xff;
}ServiceParameters_set2;

typedef struct
{
    uint8_t* buff = 0;
    uint8_t size = 0xff;
}ServiceParameters_set3;




class Device
{
private:
    uint8_t DeviceIdentifier = 0xFF;
    
public:

    virtual void init() = 0; //funkcje ktore nazucaja potomka koniecznosc ich implementacji
    virtual void cyclic() = 0;
    virtual uint8_t getDeviceIdentifier() = 0;
    virtual uint8_t getDeviceType() = 0;

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param) = 0;
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param) = 0;
   
};



#endif