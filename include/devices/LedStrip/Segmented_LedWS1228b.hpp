#ifndef LED_SEDWS1282B_TYPE_H
#define LED_SEDWS1282B_TYPE_H
#include <SystemDefinition.hpp>
#ifdef LED_STRIP_SUPPORTED

#include "devices/Device.hpp"
#include "Adafruit_NeoPixel.h"
#include "Animations/FadeIn.hpp"
#include "Animations/FadeOut.hpp"

#include "Arduino.h"

struct SegmentData {
    LedColor* current = nullptr;
    LedColor* savedSlot1 = nullptr;
    LedColor* savedSlot2 = nullptr;   
};

class SegLedWS1228bDeviceType : public Device {
    enum LedStripContentIndex{
        eACTIVE_CURRENT_CONTENT = 0,
        eSAVED_CONTENT_SLOT1,
        eSAVED_CONTENT_SLOT2,
        eDIFFERENT_CONTENTS_COUNT
    };
    /*SEG properties*/
    std::vector<uint8_t> segmentLedCount; //number of segments in each segment led strip 
    std::vector<uint8_t> segmentFlips; //flip segments in each segment led strip
    /*SEG properties*/


    uint8_t* extendedMemoryPointer = nullptr;
    std::vector<SegmentData> stripContents; //colors for each segment led strip




    LedColor averagedColors[eDIFFERENT_CONTENTS_COUNT] = {0};
    std::vector<uint8_t> segmentStatus;
    std::vector<LedColor> segmentColors; //colors for each segment

    int totalLedsCount = 0; //total number of physical leds in strip


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
    std::function<void(void)> m_reportNvmDataChangedCbk;
    Adafruit_NeoPixel* adafruit_ws2812b = nullptr;
    ILedAnimation* ongoingAnimation = nullptr;
    ILedAnimation* switchOffAnimation = nullptr;

    void applyVirtualToRealDiodes();
    void setHwLedStripColor(uint8_t virtualLedIndex, uint8_t r, uint8_t g, uint8_t b);

    void setSegmentState(uint8_t segmentIndex, uint8_t state);
    public:
    SegLedWS1228bDeviceType(DeviceConfigSlotType nvmData, std::function<void(void)> reportNvmDataChangedCbk);

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
    
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set1 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set2 param);
    virtual ServiceRequestErrorCode service(DeviceServicesType serviceType, ServiceParameters_set3 param);

};


#else /* LED STRIP NOT SUPPORTED */

#endif


#endif