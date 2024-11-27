#ifndef ONOFF_DEVICE_H
#define ONOFF_DEVICE_H

#include "Arduino.h"
#include "Device.hpp"

#include <Adafruit_NeoPixel.h>


class OnOffDevice
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

    void on();
    void off();
    bool getState(); //getter na stan urzadzenia
  
    void init(); 
    void cyclic();
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
};




#endif
