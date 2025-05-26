#include "devices/LedStrip/Segmented_LedWS1228b.hpp"
#ifdef LED_STRIP_SUPPORTED
const uint8_t maxVirtualLeds = 100;

SegLedWS1228bDeviceType::SegLedWS1228bDeviceType(DeviceConfigSlotType nvmData, std::function<void(void)> reportNvmDataChangedCbk)
{
    m_reportNvmDataChangedCbk = reportNvmDataChangedCbk;
    for(uint8_t i = 5; i < 10; i++){
        if(nvmData.customBytes[i] > 0){
            if(nvmData.customBytes[i] <= 100){
                segmentLedCount.push_back(nvmData.customBytes[i]);
            }
        }else {
            break;
        }
    }
    for(uint8_t i = 10; i < 10 + segmentLedCount.size(); i++){
        segmentFlips.push_back(nvmData.customBytes[i]);
    }

    for(uint8_t i = 0; i < segmentLedCount.size(); i++){
        segmentStatus.push_back(0); /* all segments are off by default */
    }

    for(uint8_t i = 0; i < segmentLedCount.size(); i++){
        segmentColors.push_back(LedColor(0,0,0)); /* all segments are black by default */
    }

    for(uint8_t i = 0; i < segmentLedCount.size(); i++){
        /* push empty structure to have strip content in lenght of how many segments is active*/
        stripContents.push_back(SegmentData());
    }

    for(uint8_t i = 0; i < segmentLedCount.size(); i++){
        totalLedsCount += segmentLedCount.at(i);
    }

    
    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    adafruit_ws2812b = new Adafruit_NeoPixel(
        totalLedsCount,
        pinNumber,
        NEO_GRB + NEO_KHZ800
    );

    adafruit_ws2812b->begin();

    Serial.println("SegLedWS1228bDeviceType:// Applying current limit to : " +String((int)nvmData.customBytes[0]) + " for device id : " + String((int)deviceId));
    adafruit_ws2812b->setBrightness(nvmData.customBytes[0]); /* current limiter usage */


}

ServiceRequestErrorCode SegLedWS1228bDeviceType::updateExtendedMemoryPtr(uint8_t* ptr, uint16_t size)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;
    if(ptr != nullptr && size == getExtendedMemoryLength()){
        /* Data with valid length is allocated, pointer can be used */
        extendedMemoryPointer = ptr;
        LedColor* tempPtr = (LedColor*) extendedMemoryPointer; 

        uint16_t offset = 0;
        for(uint8_t i = 0; i < stripContents.size(); i++){
            Serial.println("SegLedWS1228bDeviceType:// Initializing segment " + String((int)i) + " with " + String((int)segmentLedCount.at(i)) + " leds");
            stripContents.at(i).current = tempPtr;

            tempPtr += segmentLedCount.at(i); /* move pointer to next segment */
            stripContents.at(i).savedSlot1 = tempPtr;

            tempPtr += segmentLedCount.at(i); /* move pointer to next segment */
            stripContents.at(i).savedSlot2 = tempPtr;

            if(i < stripContents.size()-1){
                tempPtr += segmentLedCount.at(i); 
            }
        }

        Serial.println("offset : " + String((uint8_t*)tempPtr - (uint8_t*)extendedMemoryPointer));
        Serial.println("Extended memory size : " + String(getExtendedMemoryLength()));


        Serial.println("SegLedWS1228bDeviceType:// Successfully initialized");

        isContentInitialized = true;


        retVal = SERV_SUCCESS;
    }

    return retVal;
}

void SegLedWS1228bDeviceType::init(){
}

void SegLedWS1228bDeviceType::cyclic(){
    static long long animationProcessTime = 0;

    if(switchOffAnimation != nullptr){
        if(millis() - animationProcessTime > 60){
            if(switchOffAnimation->isInProgress()){
                switchOffAnimation->process();
                applyVirtualToRealDiodes();
            }else {
                switchOffAnimation->restoreColors();
                delete switchOffAnimation;
                switchOffAnimation = nullptr;
            }

            animationProcessTime = millis();
        }
    }else if(ongoingAnimation != nullptr){
        if(millis() - animationProcessTime > 60){
            if(ongoingAnimation->isInProgress()){
                ongoingAnimation->process();
                applyVirtualToRealDiodes();
            }else{
                Serial.println("ON animation completed");
                delete ongoingAnimation;
                ongoingAnimation = nullptr;
            }

            animationProcessTime = millis();
        }
    }
}

bool SegLedWS1228bDeviceType::isStripInitialized()
{
    return (adafruit_ws2812b != nullptr);
}


void SegLedWS1228bDeviceType::setColors(LedColor* ledsArray, uint16_t count)
{
    // if(isContentInitialized){
    //     if(count == virtualDiodesCount){
    //         memcpy(stripContent[eACTIVE_CURRENT_CONTENT], ledsArray, (count*sizeof(LedColor)));
    //         applyColors();

    //     }
    // }
}

void SegLedWS1228bDeviceType::getDetailedColors(LedColor* memoryBuffer, uint16_t count)
{
    if(isContentInitialized){
        if(count == virtualDiodesCount){
            // memcpy(memoryBuffer, stripContent[eACTIVE_CURRENT_CONTENT], (count*sizeof(LedColor)));
        }  
    }
}

ServiceRequestErrorCode SegLedWS1228bDeviceType::applyContent(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(isContentInitialized && contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eDIFFERENT_CONTENTS_COUNT)
    {
        Serial.println("SegLedWS1228bDeviceType:// requested load content " + String((int)contentIndex));
        // memcpy( /* copy data from requested content to active content */
        //     stripContent[eACTIVE_CURRENT_CONTENT], 
        //     stripContent[contentIndex],
        //     (virtualDiodesCount*sizeof(LedColor))
        // );

        // applyColors();
        retVal = SERV_SUCCESS;
    }

    return retVal;
}


ServiceRequestErrorCode SegLedWS1228bDeviceType::saveContentAs(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(isContentInitialized && contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eDIFFERENT_CONTENTS_COUNT)
    {
        Serial.println("SegLedWS1228bDeviceType:// requested overwrite content " + String((int)contentIndex));
        Serial.println("contentIndex : " + String((int)contentIndex));
        // memcpy( /* copy data from active content to choosen content slot */ 
        //     stripContent[contentIndex],
        //     stripContent[eACTIVE_CURRENT_CONTENT],
        //     (virtualDiodesCount*sizeof(LedColor))
        // );

        Serial.println("Slot saved");

        // averagedColors[contentIndex] = averagedColors[eACTIVE_CURRENT_CONTENT];

        Serial.println("memoryAveragedColors saved");

        retVal = SERV_SUCCESS;

        /* this action will affect NVM data */
        if(m_reportNvmDataChangedCbk){
            m_reportNvmDataChangedCbk();
        }
    }

    return retVal;
}

void SegLedWS1228bDeviceType::stripOn()
{
    // applyColors();
}

void SegLedWS1228bDeviceType::stripOff()
{
    if(switchOffAnimation != nullptr){
        delete switchOffAnimation;
    }
    // switchOffAnimation = new FadeOutAnimation(
    //     stripContent[eACTIVE_CURRENT_CONTENT],
    //     virtualDiodesCount
    // );
    switchOffAnimation->start(false);
}

bool notBlack(LedColor color){
    return !(color.r == 0 && color.g == 0 && color.b == 0);
}


void SegLedWS1228bDeviceType::setSegmentState(uint8_t segmentIndex, uint8_t state)
{
    if(segmentIndex < segmentStatus.size()){
        segmentStatus[segmentIndex] = state;
        if(state == 0){
            Serial.println("SegLedWS1228bDeviceType:// Segment " + String((int)segmentIndex) + " turned off");
            segmentColors[segmentIndex] = LedColor(0,0,0); /* turn off segment */
            isOn = false; /* turn off whole strip */
            //setColors(&LedColor(0,0,0), virtualDiodesCount);
        }else {
            Serial.println("SegLedWS1228bDeviceType:// Segment " + String((int)segmentIndex) + " turned on");
            segmentColors[segmentIndex] =  LedColor(0, 0,255); /* set segment color to averaged color */
            isOn = true; /* turn on whole strip */
            //setColors(&segmentColors[segmentIndex], virtualDiodesCount);
        }
    }
}


uint16_t SegLedWS1228bDeviceType::getExtendedMemoryLength(){
    return ((totalLedsCount * eDIFFERENT_CONTENTS_COUNT) * sizeof(LedColor));
}

uint8_t SegLedWS1228bDeviceType::getDeviceIdentifier(){
    return deviceId;
}
uint8_t SegLedWS1228bDeviceType::getDeviceType(){
    return type_LED_STRIP_SEGMENTED;
}

ServiceRequestErrorCode SegLedWS1228bDeviceType::service(DeviceServicesType serviceType){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode SegLedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        case DEVSERVICE_SEGMENT_STATE_SWITCH:
            /* Switch segment state */
            setSegmentState(param.a, param.b);
            return SERV_SUCCESS;

        break;

        case DEVSERVICE_STATE_SWITCH:
            for(uint8_t i = 0; i < segmentStatus.size(); i++){
                setSegmentState(i, param.a);
            }
            if(param.a == 1) {
                isOn = true;
            }
            else {
                isOn = false;
            }   
            return SERV_SUCCESS;

        case DEVSERVICE_LED_STRIP_SAVE_CONTENT:
            Serial.println("SegLedWS1228bDeviceType://DEVSERVICE_LED_STRIP_SAVE_CONTENT");
            return SERV_GENERAL_FAILURE; //saveContentAs((LedStripContentIndex)param.a);
        
        case DEVSERVICE_LED_STRIP_SWITCH_CONTENT:
            Serial.println("SegLedWS1228bDeviceType://DEVSERVICE_LED_STRIP_SWITCH_CONTENT");
            return SERV_GENERAL_FAILURE; //applyContent((LedStripContentIndex)param.a);

        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode SegLedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode SegLedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        case DEVSERVICE_GET_ADVANCED_CONTROLS:
        case DEVSERVICE_GET_DETAILED_COLORS:
            // Serial.println("Advanced controls requested");
            // if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor)))
            // {
            //     getDetailedColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
            //     return SERV_SUCCESS;
            // }else {
            //     return SERV_EXECUTION_FAILURE;
            // }
            return SERV_GENERAL_FAILURE; //getDetailedColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
        case DEVSERVICE_SET_DETAILED_COLORS:
            Serial.println("param.size: " + String((int)param.size) + ", must be : " + String((int)virtualDiodesCount*sizeof(LedColor)));
            // if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor)))
            // {
            //     Serial.println("Color change requested");
            //     setColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));

            //     /* this action will affect NVM data */
            //     if(m_reportNvmDataChangedCbk){
            //         m_reportNvmDataChangedCbk();
            //     }
            //     return SERV_SUCCESS;
            // }else {
            //     return SERV_EXECUTION_FAILURE;
            // }
            return SERV_GENERAL_FAILURE; //setColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));

        case DEVSERVICE_SET_EXT_MEMORY_PTR:
            return updateExtendedMemoryPtr(param.buff, param.size);
        default: 
            return SERV_NOT_SUPPORTED;
    };
}

void SegLedWS1228bDeviceType::applyVirtualToRealDiodes()
{
    for(uint8_t i = 0; i < virtualDiodesCount ; i ++){
        uint8_t adjustedIdx = isInversedOrder ? (virtualDiodesCount-(i+1)) : i;
        // setHwLedStripColor(
        //     i,
        //     stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].r,
        //     stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].g,
        //     stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].b
        // );
    }

    adafruit_ws2812b->show();
}


void SegLedWS1228bDeviceType::setHwLedStripColor(
    uint8_t virtualLedIndex,
    uint8_t r,
    uint8_t g,
    uint8_t b
){
    if(virtualDiodesCount == diodesCount){
        adafruit_ws2812b->setPixelColor(
            virtualLedIndex,
            adafruit_ws2812b->Color(
                r,
                g,
                b
            )
        );
    }else {
        uint8_t reminderAdjustment = 0;
        if(virtualLedIndex > (virtualDiodesCount/2)){
            reminderAdjustment = diodesCount % physicalLedsPerVirtualLed;
        }else if(virtualLedIndex == (virtualDiodesCount/2)){
            reminderAdjustment = diodesCount % physicalLedsPerVirtualLed;
            for(uint8_t j = 0; j < reminderAdjustment; j++){
                adafruit_ws2812b->setPixelColor(
                    (virtualLedIndex * physicalLedsPerVirtualLed + j),
                    adafruit_ws2812b->Color(
                        r,
                        g,
                        b
                    )
                );
            }
        }

        for(uint8_t j = 0; j < physicalLedsPerVirtualLed; j++){
            adafruit_ws2812b->setPixelColor(
                (virtualLedIndex * physicalLedsPerVirtualLed + reminderAdjustment + j),
                adafruit_ws2812b->Color(
                    r,
                    g,
                    b
                )
            );
        }
    }
    

}

DeviceDescription SegLedWS1228bDeviceType::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = isOn;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

    
    desc.customBytes[0] = segmentLedCount.size(); /* number of segments */

    /* byte 1 -6 : segments status*/
    for(uint8_t status = 0; status < segmentStatus.size(); status++){
        desc.customBytes[1+status] = segmentStatus[status];
    }

    /* byte 6 - 20 : segments colors */
    for(uint8_t color = 0; color < segmentColors.size(); color++){
        desc.customBytes[7 + (color * 3)] = segmentColors[color].r;
        desc.customBytes[8 + (color * 3)] = segmentColors[color].g;
        desc.customBytes[9 + (color * 3)] = segmentColors[color].b;
    }

    // /* number of segments */
    // desc.customBytes[0] = segmentLedCount.size();

    // desc.customBytes[2] = averagedColors[eACTIVE_CURRENT_CONTENT].r; // average color R
    // desc.customBytes[3] = averagedColors[eACTIVE_CURRENT_CONTENT].g; // average color G
    // desc.customBytes[4] = averagedColors[eACTIVE_CURRENT_CONTENT].b; // average color B

    // desc.customBytes[5] = averagedColors[eSAVED_CONTENT_SLOT1].r; // average color R
    // desc.customBytes[6] = averagedColors[eSAVED_CONTENT_SLOT1].g; // average color G
    // desc.customBytes[7] = averagedColors[eSAVED_CONTENT_SLOT1].b; // average color B

    // desc.customBytes[8] = averagedColors[eSAVED_CONTENT_SLOT2].r; // average color R
    // desc.customBytes[9] = averagedColors[eSAVED_CONTENT_SLOT2].g; // average color G
    // desc.customBytes[10] = averagedColors[eSAVED_CONTENT_SLOT2].b; // average color B

    // desc.customBytes[11] = averagedColors[eSAVED_CONTENT_SLOT3].r; // average color R
    // desc.customBytes[12] = averagedColors[eSAVED_CONTENT_SLOT3].g; // average color G
    // desc.customBytes[13] = averagedColors[eSAVED_CONTENT_SLOT3].b; // average color B

    return desc;
}

#endif