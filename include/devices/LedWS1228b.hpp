#ifndef LED_WS1282B_TYPE_H
#define LED_WS1282B_TYPE_H
#include "devices/Device.hpp"
#include "Arduino.h"
#include "Adafruit_NeoPixel.h"


class LedWS1228bDeviceType : public Device {

    bool isOn = false; //stan urzadzenia
    int pinNumber;
    uint8_t brightness = 0;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    Adafruit_NeoPixel* adafruit_ws2812b = nullptr;
    public:
    LedWS1228bDeviceType(DeviceConfigSlotType nvmData);

    virtual void init();
    virtual void cyclic();
    virtual uint8_t getDeviceIdentifier();
    virtual uint8_t getDeviceType();
    virtual DeviceDescription getDeviceDescription();

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);

};



#endif