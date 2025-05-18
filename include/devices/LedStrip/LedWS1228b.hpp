#ifndef LED_WS1282B_TYPE_H
#define LED_WS1282B_TYPE_H
#include <SystemDefinition.hpp>
#ifdef LED_STRIP_SUPPORTED

#include "devices/Device.hpp"
#include "Adafruit_NeoPixel.h"
#include "Animations/FadeIn.hpp"
#include "Animations/FadeOut.hpp"

#include "Arduino.h"



class LedWS1228bDeviceType : public Device {
    enum LedStripContentIndex{
        eACTIVE_CURRENT_CONTENT = 0,
        eSAVED_CONTENT_SLOT1,
        eSAVED_CONTENT_SLOT2,
        eSAVED_CONTENT_SLOT3,
        eDIFFERENT_CONTENTS_COUNT
    };

    uint8_t* extendedMemoryPointer = nullptr;
    LedColor* stripContent[4];
    LedColor averagedColors[4] = {0};
    bool isContentInitialized = false;
    bool isInversedOrder = false;
    bool isOn = false; //stan urzadzenia
    uint16_t diodesCount = 0;
    uint8_t virtualDiodesCount = 255;
    uint8_t physicalLedsPerVirtualLed = 1;
    int pinNumber;
    uint8_t brightness = 0;
    uint8_t deviceId;
    String deviceName;
    uint8_t roomId;
    Adafruit_NeoPixel* adafruit_ws2812b = nullptr;
    ILedAnimation* ongoingAnimation = nullptr;
    ILedAnimation* switchOffAnimation = nullptr;

    void applyVirtualToRealDiodes();
    void setHwLedStripColor(uint8_t virtualLedIndex, uint8_t r, uint8_t g, uint8_t b);
    public:
    LedWS1228bDeviceType(DeviceConfigSlotType nvmData);

    virtual void init();
    virtual void cyclic();
    virtual uint8_t getDeviceIdentifier();
    virtual uint8_t getDeviceType();
    virtual DeviceDescription getDeviceDescription();
    virtual uint16_t getExtendedMemoryLength();
    bool isStripInitialized();

    ServiceRequestErrorCode updateExtendedMemoryPtr(uint8_t* ptr, uint16_t size);
    void stripOn();
    void stripOff();
    ServiceRequestErrorCode applyContent(LedStripContentIndex contentIndex);
    ServiceRequestErrorCode saveContentAs(LedStripContentIndex contentIndex);

    void setColors(LedColor* ledsArray, uint16_t count);
    void getDetailedColors(LedColor* memoryBuffer, uint16_t count);

    void applyColors();
    

    void updateAveragedColor(LedStripContentIndex content);

    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);

};


#else /* LED STRIP NOT SUPPORTED */

#endif


#endif