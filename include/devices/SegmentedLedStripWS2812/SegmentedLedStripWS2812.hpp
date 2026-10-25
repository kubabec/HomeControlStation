#ifndef SEGMENTED_LED_STRIP_WS2812_H
#define SEGMENTED_LED_STRIP_WS2812_H
#include <SystemDefinition.hpp>
#ifdef LED_STRIP_SUPPORTED

#include "devices/device.hpp"
#include "Adafruit_NeoPixel.h"
#include "devices/LedStrip/Animations/FadeIn.hpp"
#include "devices/LedStrip/Animations/FadeOut.hpp"
#include "devices/LedStrip/Animations/RollInAnimation.hpp"
#include "devices/LedStrip/Animations/RollOutAnimation.hpp"
#include "devices/LedStrip/Animations/BounceInAnimation.hpp"
#include "devices/LedStrip/Animations/FallingStarsAnimation.hpp"
#include "devices/LedStrip/Animations/ColorShuffleAnimation.hpp"
#include "devices/LedStrip/Animations/BlinkingSkyAnimation.hpp"
#include "devices/LedStrip/Animations/SupernovaAnimation.hpp"
#include "devices/LedStrip/Animations/live/SingleWaveAnimation.hpp"

#include "Arduino.h"

#ifdef SUPERMINI
#define SEGMENTED_STRIP_MAX_SEGMENTS 4
#define SEGMENTED_STRIP_MAX_VIRTUAL_LEDS 50
#else
#define SEGMENTED_STRIP_MAX_SEGMENTS 8
#define SEGMENTED_STRIP_MAX_VIRTUAL_LEDS 100
#endif

/** Number of property slots always present in the advanced-controls payload and extended memory. */
#define SEGMENTED_STRIP_PROPERTY_SLOTS 8
/** Segment selector meaning "every segment" in service parameters. */
#define SEGMENTED_STRIP_ALL_SEGMENTS 0xFF

/**
 * @class SegmentedLedStripWS2812
 * @brief One WS2812B chain virtually split into independent segments.
 *
 * Every segment owns its colors, on/off state, animation properties and animations.
 * The strip is limited to SEGMENTED_STRIP_MAX_VIRTUAL_LEDS virtual diodes in total;
 * when more physical diodes are configured, neighbouring diodes are merged into one virtual diode.
 */
class SegmentedLedStripWS2812 : public Device
{
    struct Segment
    {
        uint16_t physicalOffset = 0;
        uint16_t physicalCount = 0;
        uint8_t virtualOffset = 0;
        uint8_t virtualCount = 0;
        bool isInversed = false;
        bool isOn = false;
        bool pendingLiveStart = false;
        LedColor averageColor;
        LedStripAnimationProperties properties;
        uint8_t animationWaitTicks = 4;
        int ticksToAnimate = 4;
        int liveTicksToAnimate = 0;
        ILedAnimation* ongoingAnimation = nullptr;
        ILedAnimation* switchOffAnimation = nullptr;
        ILiveAnimation* liveAnimation = nullptr;
    };

    Segment segments[SEGMENTED_STRIP_MAX_SEGMENTS];
    uint8_t segmentCount = 0;
    uint16_t physicalLedsCount = 0;
    uint8_t physicalLedsPerVirtualLed = 1;
    uint8_t virtualLedsCount = 0;

    uint8_t* extendedMemoryPointer = nullptr;
    LedColor* content = nullptr;       /* persistent colors, lives in extended memory */
    LedColor* runtimeBuffer = nullptr; /* colors currently shown, modified by animations */
    bool isContentInitialized = false;
    bool isHardwareDirty = false;

    uint8_t pinNumber = 255;
    uint8_t deviceId = 255;
    uint8_t roomId = 255;
    String deviceName;
    std::function<void(void)> m_reportNvmDataChangedCbk;
    Adafruit_NeoPixel* adafruit_ws2812b = nullptr;

    static uint8_t calculatePhysicalPerVirtual(const uint8_t* counts, uint8_t segmentsNumber);

    LedColor* window(const Segment& segment) { return runtimeBuffer + segment.virtualOffset; }
    LedColor* contentWindow(const Segment& segment) { return content + segment.virtualOffset; }
    uint8_t* propertiesStorage();
    void saveProperties(uint8_t segmentIndex);
    void reportNvmChanged();

    bool isSegmentBusy(const Segment& segment) const;
    bool isAnySegmentOn() const;
    bool isAnySegmentBusy() const;
    void updateAverageColor(Segment& segment);
    void updateAnimationSpeed(Segment& segment);
    void createEnablingAnimation(Segment& segment);
    void createDisablingAnimation(Segment& segment);
    void createLiveAnimation(Segment& segment);
    void stopLiveAnimation(Segment& segment, bool restoreContent);
    void releaseAnimations(Segment& segment);

    void segmentOn(Segment& segment);
    void segmentOff(Segment& segment);
    void startLive(Segment& segment);
    void processSegment(Segment& segment);
    void applyColorsToSegment(Segment& segment);
    void renderToHardware();

    ServiceRequestErrorCode switchSegments(uint8_t segmentIndex, uint8_t state);
    ServiceRequestErrorCode liveSegments(uint8_t segmentIndex, uint8_t state);
    ServiceRequestErrorCode getAdvancedControls(ServiceParameters_set3& param);
    ServiceRequestErrorCode setAdvancedControls(ServiceParameters_set3& param);
    ServiceRequestErrorCode updateExtendedMemoryPtr(uint8_t* ptr, uint16_t size);

public:
    SegmentedLedStripWS2812(DeviceConfigSlotType nvmData, std::function<void(void)> reportNvmDataChangedCbk);
    virtual ~SegmentedLedStripWS2812();

    /**
     * Configuration validator used by the generated registry.
     * Rejects configurations whose segment sizes do not add up to the total diode count.
     */
    static bool isConfigurationValid(const DeviceConfigSlotType& config);

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

#endif /* LED_STRIP_SUPPORTED */

#endif
