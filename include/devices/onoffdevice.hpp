#ifndef ONOFF_DEVICE_H
#define ONOFF_DEVICE_H

#include "Arduino.h"
#include "Device.hpp"


class OnOffDevice : public Device
{
private:
    bool isOn = false; //stan urzadzenia
    int pinNumber;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    bool brightnessLevelSupport = true; //czy obslugiwana jest zmiana jasnosci
    int brightnessLevel = 30;
    int brightnessLevelTarget = 30;
    const int brightnessChangeTime = 3000;
    int lightDurationTimerMS = 8000;

    unsigned long brighnessStep ;
    unsigned long brightnessStepDurationMS ;
    unsigned long timePrevious1 ;
    unsigned long timePrevious2 ;
    
    
public:
    OnOffDevice(int pin, String devName, uint8_t a_deviceId, uint8_t a_roomId);
    OnOffDevice(DeviceDescription& description, uint8_t pin);

    void on();
    void off();
    bool getState(); //getter na stan urzadzenia  
    
    void setBrightnessLevelSupport(bool p_brightnessLevelSupport); //ustawienie czy obslugiwana jest zmiana jasnosci
    void brightnessChangeHandler();
    int getBrightnessLevel();
    int getBrighnessStep();
    int getBrightnessStepDuration();
    void changeBrightness(int requestedBrightness);
    int getBrightnessLevelTarget();
    void setLightDurationTimerMS(int requestedLightDuration);
    void timerHandler();
    int getLightDurationTimerMS();
    String getName();
    uint8_t getDeviceId();
    uint8_t getRoomId();
    bool getBrightnessIsAdjustable();

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
