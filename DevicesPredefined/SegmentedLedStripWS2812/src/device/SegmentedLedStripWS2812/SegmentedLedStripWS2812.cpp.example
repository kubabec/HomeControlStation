#include "devices/SegmentedLedStripWS2812/SegmentedLedStripWS2812.hpp"
#include <vector>
#include "generated/GeneratedEnablingConditions.hpp"
#include "os/Logger.hpp"
#ifdef LED_STRIP_SUPPORTED
static_assert(sizeof(LedStripAnimationProperties) == 5, "LED advanced-controls property size is out of sync");
static_assert(sizeof(LedColor) == 3, "LED advanced-controls element size is out of sync");

/* Configuration byte layout */
#define CFG_CURRENT_LIMIT 2
#define CFG_COLOR_ORDER 3
#define CFG_SEGMENT_COUNT 4
#define CFG_SEGMENT_LEDS 5 /* 8 bytes, one per segment */
#define CFG_FLIP_MASK 13
#define CFG_CURRENT_LIMIT_COPY 19

#define PROPERTIES_SIZE (SEGMENTED_STRIP_PROPERTY_SLOTS * sizeof(LedStripAnimationProperties))
#define LIVE_WAIT_TICKS 7

static neoPixelType stripColorOrder(uint8_t configured)
{
    switch (configured)
    {
    case 1:
        return NEO_RGB;
    case 2:
        return NEO_BRG;
    case 3:
        return NEO_RBG;
    case 4:
        return NEO_GBR;
    case 5:
        return NEO_BGR;
    default:
        return NEO_GRB;
    }
}

bool SegmentedLedStripWS2812::isConfigurationValid(const DeviceConfigSlotType &config)
{
    uint16_t total = 0;
    memcpy(&total, &(config.customBytes[0]), sizeof(uint16_t));
    const uint8_t count = config.customBytes[CFG_SEGMENT_COUNT];

    if (count < 1 || count > SEGMENTED_STRIP_MAX_SEGMENTS)
    {
        return false;
    }

    if (config.customBytes[CFG_CURRENT_LIMIT] != config.customBytes[CFG_CURRENT_LIMIT_COPY])
    {
        return false;
    }

    uint16_t sum = 0;
    for (uint8_t i = 0; i < count; i++)
    {
        const uint8_t leds = config.customBytes[CFG_SEGMENT_LEDS + i];
        if (leds == 0)
        {
            return false;
        }
        sum += leds;
    }

    return total > 0 && sum == total;
}

/* Smallest number of physical diodes merged into one virtual diode so that all segments fit the virtual limit */
uint8_t SegmentedLedStripWS2812::calculatePhysicalPerVirtual(const uint8_t *counts, uint8_t segmentsNumber)
{
    for (uint8_t perVirtual = 1; perVirtual < 255; perVirtual++)
    {
        uint16_t virtualTotal = 0;
        for (uint8_t i = 0; i < segmentsNumber; i++)
        {
            virtualTotal += (counts[i] + perVirtual - 1) / perVirtual;
        }
        if (virtualTotal <= SEGMENTED_STRIP_MAX_VIRTUAL_LEDS)
        {
            return perVirtual;
        }
    }
    return 255;
}

SegmentedLedStripWS2812::SegmentedLedStripWS2812(DeviceConfigSlotType nvmData, std::function<void(void)> reportNvmDataChangedCbk)
{
    m_reportNvmDataChangedCbk = reportNvmDataChangedCbk;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    if (!isConfigurationValid(nvmData))
    {
        /* the registry never creates such a device, this only protects direct construction */
        return;
    }

    segmentCount = nvmData.customBytes[CFG_SEGMENT_COUNT];
    memcpy(&physicalLedsCount, &(nvmData.customBytes[0]), sizeof(uint16_t));
    physicalLedsPerVirtualLed = calculatePhysicalPerVirtual(&nvmData.customBytes[CFG_SEGMENT_LEDS], segmentCount);

    uint16_t physicalOffset = 0;
    uint8_t virtualOffset = 0;
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        Segment &segment = segments[i];
        segment.physicalOffset = physicalOffset;
        segment.physicalCount = nvmData.customBytes[CFG_SEGMENT_LEDS + i];
        segment.virtualOffset = virtualOffset;
        segment.virtualCount = (segment.physicalCount + physicalLedsPerVirtualLed - 1) / physicalLedsPerVirtualLed;
        segment.isInversed = (nvmData.customBytes[CFG_FLIP_MASK] >> i) & 0x01;
        physicalOffset += segment.physicalCount;
        virtualOffset += segment.virtualCount;
    }
    virtualLedsCount = virtualOffset;

    adafruit_ws2812b = new Adafruit_NeoPixel(
        physicalLedsCount,
        pinNumber,
        stripColorOrder(nvmData.customBytes[CFG_COLOR_ORDER]) + NEO_KHZ800);
    adafruit_ws2812b->begin();

    /* the current limit is stored twice, a mismatch is rejected by the validator */
    adafruit_ws2812b->setBrightness(nvmData.customBytes[CFG_CURRENT_LIMIT]);
}

SegmentedLedStripWS2812::~SegmentedLedStripWS2812()
{
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        releaseAnimations(segments[i]);
    }
    free(runtimeBuffer);
    delete adafruit_ws2812b;
}

uint16_t SegmentedLedStripWS2812::getExtendedMemoryLength()
{
    return (virtualLedsCount * sizeof(LedColor)) + PROPERTIES_SIZE;
}

uint8_t *SegmentedLedStripWS2812::propertiesStorage()
{
    return extendedMemoryPointer + (virtualLedsCount * sizeof(LedColor));
}

void SegmentedLedStripWS2812::saveProperties(uint8_t segmentIndex)
{
    if (extendedMemoryPointer != nullptr && segmentIndex < SEGMENTED_STRIP_PROPERTY_SLOTS)
    {
        memcpy(propertiesStorage() + segmentIndex * sizeof(LedStripAnimationProperties),
               &segments[segmentIndex].properties, sizeof(LedStripAnimationProperties));
    }
}

void SegmentedLedStripWS2812::reportNvmChanged()
{
    if (m_reportNvmDataChangedCbk)
    {
        m_reportNvmDataChangedCbk();
    }
}

ServiceRequestErrorCode SegmentedLedStripWS2812::updateExtendedMemoryPtr(uint8_t *ptr, uint16_t size)
{
    if (segmentCount == 0 || ptr == nullptr || size != getExtendedMemoryLength())
    {
        return SERV_EXECUTION_FAILURE;
    }

    extendedMemoryPointer = ptr;
    content = (LedColor *)extendedMemoryPointer;

    free(runtimeBuffer);
    runtimeBuffer = (LedColor *)malloc(virtualLedsCount * sizeof(LedColor));
    if (runtimeBuffer == nullptr)
    {
        return SERV_EXECUTION_FAILURE;
    }
    memcpy(runtimeBuffer, content, virtualLedsCount * sizeof(LedColor));

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        memcpy(&segments[i].properties,
               propertiesStorage() + i * sizeof(LedStripAnimationProperties),
               sizeof(LedStripAnimationProperties));
        updateAverageColor(segments[i]);
    }

    isContentInitialized = true;
    return SERV_SUCCESS;
}

void SegmentedLedStripWS2812::init()
{
}

uint8_t SegmentedLedStripWS2812::getDeviceIdentifier()
{
    return deviceId;
}

uint8_t SegmentedLedStripWS2812::getDeviceType()
{
    return type_SEGMENTED_LED_STRIP;
}

bool SegmentedLedStripWS2812::isSegmentBusy(const Segment &segment) const
{
    return segment.ongoingAnimation != nullptr || segment.switchOffAnimation != nullptr || segment.pendingLiveStart;
}

bool SegmentedLedStripWS2812::isAnySegmentBusy() const
{
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        if (isSegmentBusy(segments[i]))
        {
            return true;
        }
    }
    return false;
}

bool SegmentedLedStripWS2812::isAnySegmentOn() const
{
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        if (segments[i].isOn)
        {
            return true;
        }
    }
    return false;
}

void SegmentedLedStripWS2812::updateAverageColor(Segment &segment)
{
    int sumR = 0, sumG = 0, sumB = 0, lit = 0;
    const LedColor *colors = contentWindow(segment);

    for (uint8_t i = 0; i < segment.virtualCount; i++)
    {
        if (colors[i].r != 0 || colors[i].g != 0 || colors[i].b != 0)
        {
            sumR += colors[i].r;
            sumG += colors[i].g;
            sumB += colors[i].b;
            lit++;
        }
    }

    if (lit == 0)
    {
        segment.averageColor = {0, 0, 0};
        return;
    }
    segment.averageColor.r = (uint8_t)(sumR / lit);
    segment.averageColor.g = (uint8_t)(sumG / lit);
    segment.averageColor.b = (uint8_t)(sumB / lit);
}

void SegmentedLedStripWS2812::updateAnimationSpeed(Segment &segment)
{
    switch (segment.properties.animationSpeed)
    {
    case 0:
        segment.animationWaitTicks = 3;
        break;
    case 1:
        segment.animationWaitTicks = 1;
        break;
    case 2:
        segment.animationWaitTicks = 6;
        break;
    }
    segment.ticksToAnimate = segment.animationWaitTicks;
}

void SegmentedLedStripWS2812::createEnablingAnimation(Segment &segment)
{
    LedColor *buffer = window(segment);
    const uint16_t count = segment.virtualCount;

    switch (segment.properties.enableAnimation)
    {
    case 1:
        segment.ongoingAnimation = new RollInAnimation(buffer, count, Direction::RightToLeft);
        break;
    case 2:
        segment.ongoingAnimation = new FadeInAnimation(buffer, count);
        break;
    case 3:
        segment.ongoingAnimation = new BounceInAnimation(buffer, count, (count / 2));
        break;
    case 4:
        segment.ongoingAnimation = new FallingStarsAnimation(buffer, count);
        break;
    case 5:
        segment.ongoingAnimation = new ColorShuffleAnimation(buffer, count);
        break;
    default:
        segment.ongoingAnimation = new RollInAnimation(buffer, count, Direction::LeftToRight);
        break;
    }

    updateAnimationSpeed(segment);
}

void SegmentedLedStripWS2812::createDisablingAnimation(Segment &segment)
{
    LedColor *buffer = window(segment);
    const uint16_t count = segment.virtualCount;

    switch (segment.properties.disableAnimation)
    {
    case 1:
        segment.switchOffAnimation = new RollOutAnimation(buffer, count, Direction::LeftToRight);
        break;
    case 2:
        segment.switchOffAnimation = new RollOutAnimation(buffer, count, Direction::RightToLeft);
        break;
    case 3:
        segment.switchOffAnimation = new BlinkingSkyAnimation(buffer, count);
        break;
    case 4:
        segment.switchOffAnimation = new SupernovaAnimation(buffer, count);
        break;
    default:
        segment.switchOffAnimation = new FadeOutAnimation(buffer, count);
        break;
    }

    updateAnimationSpeed(segment);
}

void SegmentedLedStripWS2812::createLiveAnimation(Segment &segment)
{
    /* live animation is forbidden while the enabling animation is running */
    if (segment.ongoingAnimation != nullptr || !segment.isOn)
    {
        return;
    }

    delete segment.liveAnimation;
    segment.liveAnimation = new ComplexSequenceAnimation();
    segment.liveAnimation->initialize(window(segment), segment.virtualCount);
    segment.liveAnimation->start();
    segment.liveTicksToAnimate = LIVE_WAIT_TICKS;
}

void SegmentedLedStripWS2812::stopLiveAnimation(Segment &segment, bool restoreContent)
{
    if (segment.liveAnimation != nullptr)
    {
        segment.liveAnimation->stop();
        delete segment.liveAnimation;
        segment.liveAnimation = nullptr;

        if (restoreContent)
        {
            memcpy(window(segment), contentWindow(segment), segment.virtualCount * sizeof(LedColor));
        }
        isHardwareDirty = true;
    }
}

void SegmentedLedStripWS2812::releaseAnimations(Segment &segment)
{
    delete segment.ongoingAnimation;
    delete segment.switchOffAnimation;
    delete segment.liveAnimation;
    segment.ongoingAnimation = nullptr;
    segment.switchOffAnimation = nullptr;
    segment.liveAnimation = nullptr;
}

/* Replays the enabling animation on the segment using its current content */
void SegmentedLedStripWS2812::applyColorsToSegment(Segment &segment)
{
    stopLiveAnimation(segment, false);
    releaseAnimations(segment);

    memcpy(window(segment), contentWindow(segment), segment.virtualCount * sizeof(LedColor));
    updateAverageColor(segment);

    createEnablingAnimation(segment);
    segment.ongoingAnimation->start(!segment.isOn);
    segment.isOn = true;
    isHardwareDirty = true;
}

void SegmentedLedStripWS2812::segmentOn(Segment &segment)
{
    if (!segment.isOn)
    {
        applyColorsToSegment(segment);
    }
}

void SegmentedLedStripWS2812::segmentOff(Segment &segment)
{
    if (!segment.isOn)
    {
        return;
    }

    /* live animation is stopped synchronously, the content is restored before fading out */
    stopLiveAnimation(segment, true);
    delete segment.switchOffAnimation;
    createDisablingAnimation(segment);
    segment.switchOffAnimation->start(false);
    segment.isOn = false;
}

void SegmentedLedStripWS2812::startLive(Segment &segment)
{
    if (segment.liveAnimation != nullptr)
    {
        return;
    }

    if (!segment.isOn)
    {
        segment.pendingLiveStart = true;
        segmentOn(segment);
    }
    else
    {
        createLiveAnimation(segment);
    }
}

void SegmentedLedStripWS2812::processSegment(Segment &segment)
{
    if (segment.switchOffAnimation != nullptr)
    {
        if (segment.ticksToAnimate == 0)
        {
            if (segment.switchOffAnimation->isInProgress())
            {
                segment.switchOffAnimation->process();
            }
            else
            {
                segment.switchOffAnimation->restoreColors();
                delete segment.switchOffAnimation;
                segment.switchOffAnimation = nullptr;
            }
            isHardwareDirty = true;
            segment.ticksToAnimate = segment.animationWaitTicks;
        }
        else
        {
            segment.ticksToAnimate--;
        }
    }
    else if (segment.ongoingAnimation != nullptr)
    {
        if (segment.ticksToAnimate == 0)
        {
            if (segment.ongoingAnimation->isInProgress())
            {
                segment.ongoingAnimation->process();
            }
            else
            {
                delete segment.ongoingAnimation;
                segment.ongoingAnimation = nullptr;
            }
            isHardwareDirty = true;
            segment.ticksToAnimate = segment.animationWaitTicks;
        }
        else
        {
            segment.ticksToAnimate--;
        }
    }
    else if (segment.liveAnimation != nullptr)
    {
        if (segment.liveTicksToAnimate == 0)
        {
            if (segment.liveAnimation->isRunning())
            {
                segment.liveAnimation->process();
            }
            else
            {
                delete segment.liveAnimation;
                segment.liveAnimation = nullptr;
            }
            isHardwareDirty = true;
            segment.liveTicksToAnimate = LIVE_WAIT_TICKS;
        }
        else
        {
            segment.liveTicksToAnimate--;
        }
    }

    if (segment.pendingLiveStart && segment.ongoingAnimation == nullptr && segment.switchOffAnimation == nullptr)
    {
        segment.pendingLiveStart = false;
        createLiveAnimation(segment);
    }
}

void SegmentedLedStripWS2812::renderToHardware()
{
    for (uint8_t s = 0; s < segmentCount; s++)
    {
        const Segment &segment = segments[s];
        const bool visible = segment.isOn || segment.switchOffAnimation != nullptr;
        const LedColor *colors = window(segment);

        for (uint8_t j = 0; j < segment.virtualCount; j++)
        {
            const uint8_t source = segment.isInversed ? (segment.virtualCount - 1 - j) : j;
            const uint32_t color = visible
                                       ? adafruit_ws2812b->Color(colors[source].r, colors[source].g, colors[source].b)
                                       : 0;

            /* the last virtual diode of a segment takes the remaining physical diodes */
            const uint16_t first = segment.physicalOffset + (uint16_t)j * physicalLedsPerVirtualLed;
            const uint16_t end = (j == segment.virtualCount - 1)
                                     ? (segment.physicalOffset + segment.physicalCount)
                                     : (first + physicalLedsPerVirtualLed);
            for (uint16_t p = first; p < end; p++)
            {
                adafruit_ws2812b->setPixelColor(p, color);
            }
        }
    }

    adafruit_ws2812b->show();
}

void SegmentedLedStripWS2812::cyclic()
{
    if (!isContentInitialized)
    {
        return;
    }

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        processSegment(segments[i]);
    }

    if (isHardwareDirty)
    {
        isHardwareDirty = false;
        renderToHardware();
    }
}

ServiceRequestErrorCode SegmentedLedStripWS2812::switchSegments(uint8_t segmentIndex, uint8_t state)
{
    if (!isContentInitialized)
    {
        return SERV_EXECUTION_FAILURE;
    }

    const bool all = (segmentIndex == SEGMENTED_STRIP_ALL_SEGMENTS);
    if (!all && segmentIndex >= segmentCount)
    {
        return SERV_EXECUTION_FAILURE;
    }

    /* ON/OFF animations always finish before a new request is accepted */
    if (all ? isAnySegmentBusy() : isSegmentBusy(segments[segmentIndex]))
    {
        return SERV_BUSY;
    }

    if (state == 2)
    {
        state = (all ? isAnySegmentOn() : segments[segmentIndex].isOn) ? 0 : 1;
    }

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        if (all || i == segmentIndex)
        {
            if (state == 1)
            {
                segmentOn(segments[i]);
            }
            else
            {
                segmentOff(segments[i]);
            }
        }
    }
    return SERV_SUCCESS;
}

ServiceRequestErrorCode SegmentedLedStripWS2812::liveSegments(uint8_t segmentIndex, uint8_t state)
{
    if (!isContentInitialized)
    {
        return SERV_EXECUTION_FAILURE;
    }

    const bool all = (segmentIndex == SEGMENTED_STRIP_ALL_SEGMENTS);
    if (!all && segmentIndex >= segmentCount)
    {
        return SERV_EXECUTION_FAILURE;
    }

    if (all ? isAnySegmentBusy() : isSegmentBusy(segments[segmentIndex]))
    {
        return SERV_BUSY;
    }

    if (state == 2)
    {
        bool anyLive = false;
        for (uint8_t i = 0; i < segmentCount; i++)
        {
            if ((all || i == segmentIndex) && segments[i].liveAnimation != nullptr)
            {
                anyLive = true;
            }
        }
        state = anyLive ? 0 : 1;
    }

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        if (all || i == segmentIndex)
        {
            if (state == 1)
            {
                startLive(segments[i]);
            }
            else
            {
                stopLiveAnimation(segments[i], true);
            }
        }
    }
    return SERV_SUCCESS;
}

/* Payload: properties of 8 segments (5 bytes each), followed by colors of all virtual diodes in segment order */
ServiceRequestErrorCode SegmentedLedStripWS2812::getAdvancedControls(ServiceParameters_set3 &param)
{
    if (!isContentInitialized || param.size != (virtualLedsCount * sizeof(LedColor) + PROPERTIES_SIZE))
    {
        return SERV_EXECUTION_FAILURE;
    }

    memset(param.buff, 0x00, PROPERTIES_SIZE);
    for (uint8_t i = 0; i < segmentCount; i++)
    {
        memcpy(param.buff + i * sizeof(LedStripAnimationProperties), &segments[i].properties, sizeof(LedStripAnimationProperties));
    }
    memcpy(param.buff + PROPERTIES_SIZE, content, virtualLedsCount * sizeof(LedColor));
    return SERV_SUCCESS;
}

ServiceRequestErrorCode SegmentedLedStripWS2812::setAdvancedControls(ServiceParameters_set3 &param)
{
    if (!isContentInitialized || param.size != (virtualLedsCount * sizeof(LedColor) + PROPERTIES_SIZE))
    {
        return SERV_EXECUTION_FAILURE;
    }

    if (isAnySegmentBusy())
    {
        return SERV_BUSY;
    }

    const LedColor *received = (const LedColor *)(param.buff + PROPERTIES_SIZE);

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        Segment &segment = segments[i];
        memcpy(&segment.properties, param.buff + i * sizeof(LedStripAnimationProperties), sizeof(LedStripAnimationProperties));
        saveProperties(i);

        const LedColor *incoming = received + segment.virtualOffset;
        if (memcmp(incoming, contentWindow(segment), segment.virtualCount * sizeof(LedColor)) != 0)
        {
            memcpy(contentWindow(segment), incoming, segment.virtualCount * sizeof(LedColor));
            applyColorsToSegment(segment);
        }
    }

    reportNvmChanged();
    return SERV_SUCCESS;
}

ServiceRequestErrorCode SegmentedLedStripWS2812::service(DeviceServicesType)
{
    return SERV_NOT_SUPPORTED;
}

ServiceRequestErrorCode SegmentedLedStripWS2812::service(DeviceServicesType serviceType, ServiceParameters_set1 param)
{
    switch (serviceType)
    {
    case DEVSERVICE_STATE_SWITCH:
        Logger::log("<" + deviceName + "> Service: DEVSERVICE_STATE_SWITCH, param.a: " + String((int)param.a));
        return switchSegments(SEGMENTED_STRIP_ALL_SEGMENTS, param.a);

    case DEVSERVICE_SEGMENT_STATE_SWITCH:
        Logger::log("<" + deviceName + "> Service: DEVSERVICE_SEGMENT_STATE_SWITCH, segment: " + String((int)param.a));
        return switchSegments(param.a, param.b);

    case DEVSERVICE_LIVE_ANIMATION:
        Logger::log("<" + deviceName + "> Service: DEVSERVICE_LIVE_ANIMATION, param.a: " + String((int)param.a));
        return liveSegments(param.b, param.a);

    default:
        return SERV_NOT_SUPPORTED;
    }
}

ServiceRequestErrorCode SegmentedLedStripWS2812::service(DeviceServicesType, ServiceParameters_set2)
{
    return SERV_NOT_SUPPORTED;
}

ServiceRequestErrorCode SegmentedLedStripWS2812::service(DeviceServicesType serviceType, ServiceParameters_set3 param)
{
    if (serviceType == DEVSERVICE_CHECK_ENABLING_CONDITION)
    {
        return GeneratedEnablingConditions::evaluateService(getDeviceDescription(), param);
    }

    switch (serviceType)
    {
    case DEVSERVICE_GET_ADVANCED_CONTROLS:
    case DEVSERVICE_GET_DETAILED_COLORS:
        return getAdvancedControls(param);

    case DEVSERVICE_SET_ADVANCED_CONTROLS:
    case DEVSERVICE_SET_DETAILED_COLORS:
        return setAdvancedControls(param);

    case DEVSERVICE_SET_EXT_MEMORY_PTR:
        return updateExtendedMemoryPtr(param.buff, param.size);

    default:
        return SERV_NOT_SUPPORTED;
    }
}

/*
 * Description layout:
 * [0] virtual diodes, [1] segments, [2..9] segment on, [10..17] segment live,
 * [18..25] segment virtual diodes, [26..49] segment average colors (RGB)
 */
DeviceDescription SegmentedLedStripWS2812::getDeviceDescription()
{
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = isAnySegmentOn();
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

    desc.customBytes[0] = virtualLedsCount;
    desc.customBytes[1] = segmentCount;

    for (uint8_t i = 0; i < segmentCount; i++)
    {
        const Segment &segment = segments[i];
        desc.customBytes[2 + i] = segment.isOn ? 1 : 0;
        desc.customBytes[10 + i] = (segment.liveAnimation != nullptr || segment.pendingLiveStart) ? 1 : 0;
        desc.customBytes[18 + i] = segment.virtualCount;
        desc.customBytes[26 + i * 3] = segment.averageColor.r;
        desc.customBytes[27 + i * 3] = segment.averageColor.g;
        desc.customBytes[28 + i * 3] = segment.averageColor.b;
    }

    return desc;
}

#endif
