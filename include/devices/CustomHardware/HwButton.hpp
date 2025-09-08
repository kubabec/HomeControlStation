#ifndef HW_BUTTON_TYPE_H
#define HW_BUTTON_TYPE_H
#include "devices/Device.hpp"
#include "Arduino.h"
#include <functional>
#include <queue>

class HwButton : public Device {
    private:
    uint8_t deviceId = 255;
    uint8_t pinNumber = 255;
    bool activeHigh = 0;
    uint16_t localDeviceIdToTrigger = 255;
    uint64_t doubleClickEventId = 0;
    uint64_t longPressEventId = 0;
    uint64_t optionalSinglePressEventId = 0;
    long long eventStartTime = 0;
    long long eventEndTime = 0;
    const unsigned eventDurationTimeMs = 800;
    bool eventStarted = false;
    bool doubleClick = false;
    bool eventForceFinished = false;
    std::queue<std::pair<uint8_t, long long>> buttonActionsQueue;

    // Pointer to function which will trigger local (no MASTER needed) device to toggle
    std::function<void(uint16_t)> localDeviceTriggerFunction;
    // Pointer to function which forwards event trigger
    std::function<void(uint64_t)> eventTriggerFunction;

    static void ICACHE_RAM_ATTR interruptRouter(void* arg);

    void ICACHE_RAM_ATTR buttonAction();

    void processButtonAction();
    

    public:
    HwButton(DeviceConfigSlotType nvmData, std::function<void(uint16_t)> localToggleCbk, std::function<void(uint64_t)> eventTriggerCbk );
    HwButton(DeviceConfigSlotType nvmData, std::function<void(uint64_t)> eventTriggerCbk);
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