#include "devices/LedStrip/LedWS1228b.hpp"
#ifdef LED_STRIP_SUPPORTED
const uint8_t maxVirtualLeds = 100;

LedWS1228bDeviceType::LedWS1228bDeviceType(DeviceConfigSlotType nvmData, std::function<void(void)> reportNvmDataChangedCbk)
{
    m_reportNvmDataChangedCbk = reportNvmDataChangedCbk;
    memcpy(&diodesCount, &(nvmData.customBytes[0]), sizeof(uint16_t));

    /* calculate number of virtual diodes based on real count */
    if(diodesCount > maxVirtualLeds){
        // virtualDiodesCount = 100;
        
        while((diodesCount / physicalLedsPerVirtualLed) > maxVirtualLeds){
            physicalLedsPerVirtualLed++;
        }

        virtualDiodesCount = (diodesCount / physicalLedsPerVirtualLed);
        Serial.println("VirtualDiodesCount : " + String((int)virtualDiodesCount));
    }else {
        virtualDiodesCount = diodesCount;
    }


    
    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;
    isInversedOrder = nvmData.customBytes[2];

    adafruit_ws2812b = new Adafruit_NeoPixel(
        // nvmData.customBytes[0], /* leds count */
        // nvmData.pinNumber,
        diodesCount,
        pinNumber,
        NEO_GRB + NEO_KHZ800
    );

    adafruit_ws2812b->begin();

    Serial.println("LedWS1228bDeviceType:// Applying current limit to : " +String((int)nvmData.customBytes[3]) + " for device id : " + String((int)deviceId));

    /* safety check of doubled the same value of current limitation */
    uint8_t currentLimit = (nvmData.customBytes[3] == nvmData.customBytes[19]) ? nvmData.customBytes[3] : 0; /* if current limit is set to 0, use 100% */
    
    adafruit_ws2812b->setBrightness(currentLimit); /* current limiter usage */


}

ServiceRequestErrorCode LedWS1228bDeviceType::updateExtendedMemoryPtr(uint8_t* ptr, uint16_t size)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;
    if(ptr != nullptr && size == getExtendedMemoryLength()){
        /* Data with valid length is allocated, pointer can be used */
        extendedMemoryPointer = ptr;

        stripContent[eACTIVE_CURRENT_CONTENT] = (LedColor*) extendedMemoryPointer;
        stripContent[eSAVED_CONTENT_SLOT1] = (LedColor*) ((uint8_t*)stripContent[eACTIVE_CURRENT_CONTENT]  + (virtualDiodesCount * sizeof(LedColor)));
        stripContent[eSAVED_CONTENT_SLOT2] = (LedColor*) ((uint8_t*)stripContent[eSAVED_CONTENT_SLOT1]  + (virtualDiodesCount * sizeof(LedColor)));
        stripContent[eSAVED_CONTENT_SLOT3] = (LedColor*) ((uint8_t*)stripContent[eSAVED_CONTENT_SLOT2]  + (virtualDiodesCount * sizeof(LedColor)));

        Serial.println("LedWS1228bDeviceType:// Successfully initialized");

        updateAveragedColor(eACTIVE_CURRENT_CONTENT);
        updateAveragedColor(eSAVED_CONTENT_SLOT1);
        updateAveragedColor(eSAVED_CONTENT_SLOT2);
        updateAveragedColor(eSAVED_CONTENT_SLOT3);

        isContentInitialized = true;


        retVal = SERV_SUCCESS;
    }

    return retVal;
}

void LedWS1228bDeviceType::init(){
}

void LedWS1228bDeviceType::cyclic(){

    if(switchOffAnimation != nullptr){
        if(ticksToAnimate == 0){
            if(switchOffAnimation->isInProgress()){
                switchOffAnimation->process();
                applyVirtualToRealDiodes();
            }else {
                switchOffAnimation->restoreColors();
                delete switchOffAnimation;
                switchOffAnimation = nullptr;
            }

            ticksToAnimate = animationWaitTicks; // reset ticks to animate
        }else {
            ticksToAnimate--;
        }
    }else if(ongoingAnimation != nullptr){
        if(ticksToAnimate == 0){
            if(ongoingAnimation->isInProgress()){
                ongoingAnimation->process();
                applyVirtualToRealDiodes();
            }else{
                Serial.println("ON animation completed");
                delete ongoingAnimation;
                ongoingAnimation = nullptr;
            }

            ticksToAnimate = animationWaitTicks; // reset ticks to animate
        }else {
            ticksToAnimate--;
        }
    }
}

bool LedWS1228bDeviceType::isStripInitialized()
{
    return (adafruit_ws2812b != nullptr);
}

void LedWS1228bDeviceType::applyColors(){
    if(isContentInitialized){
        updateAveragedColor(eACTIVE_CURRENT_CONTENT);

        if(ongoingAnimation != nullptr){
            delete ongoingAnimation;
        }
        Serial.println("Starting fade in animation");
        // ongoingAnimation = new BounceInAnimation(
        //     stripContent[eACTIVE_CURRENT_CONTENT],
        //     virtualDiodesCount,
        //     35
        // );
        createEnablingAnimation();

        if(!isOn){
            ongoingAnimation->start(true); // start from beginning
        }else {
            ongoingAnimation->start(false); // start from beginning
        }
        

        isOn = true;

    }
}

void LedWS1228bDeviceType::setColors(LedColor* ledsArray, uint16_t count)
{
    if(isContentInitialized){
        if(count == virtualDiodesCount){
            memcpy(stripContent[eACTIVE_CURRENT_CONTENT], ledsArray, (count*sizeof(LedColor)));
            applyColors();

        }
    }
}

void LedWS1228bDeviceType::getDetailedColors(LedColor* memoryBuffer, uint16_t count)
{
    if(isContentInitialized){
        if(count == virtualDiodesCount){
            memcpy(memoryBuffer, stripContent[eACTIVE_CURRENT_CONTENT], (count*sizeof(LedColor)));
        }  
    }
}

ServiceRequestErrorCode LedWS1228bDeviceType::applyContent(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(isContentInitialized && contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eDIFFERENT_CONTENTS_COUNT)
    {
        Serial.println("LedWS1228bDeviceType:// requested load content " + String((int)contentIndex));
        memcpy( /* copy data from requested content to active content */
            stripContent[eACTIVE_CURRENT_CONTENT], 
            stripContent[contentIndex],
            (virtualDiodesCount*sizeof(LedColor))
        );

        applyColors();
        retVal = SERV_SUCCESS;
    }

    return retVal;
}


ServiceRequestErrorCode LedWS1228bDeviceType::saveContentAs(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(isContentInitialized && contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eDIFFERENT_CONTENTS_COUNT)
    {
        Serial.println("LedWS1228bDeviceType:// requested overwrite content " + String((int)contentIndex));
        Serial.println("contentIndex : " + String((int)contentIndex));
        memcpy( /* copy data from active content to choosen content slot */ 
            stripContent[contentIndex],
            stripContent[eACTIVE_CURRENT_CONTENT],
            (virtualDiodesCount*sizeof(LedColor))
        );

        Serial.println("Slot saved");

        averagedColors[contentIndex] = averagedColors[eACTIVE_CURRENT_CONTENT];

        Serial.println("memoryAveragedColors saved");

        retVal = SERV_SUCCESS;

        /* this action will affect NVM data */
        if(m_reportNvmDataChangedCbk){
            m_reportNvmDataChangedCbk();
        }
    }

    return retVal;
}

void LedWS1228bDeviceType::stripOn()
{
    applyColors();
}

void LedWS1228bDeviceType::stripOff()
{
    if(switchOffAnimation != nullptr){
        delete switchOffAnimation;
    }
    // switchOffAnimation = new RollOutAnimation(
    //     stripContent[eACTIVE_CURRENT_CONTENT],
    //     virtualDiodesCount
    // );
    createDisablingAnimation();
    switchOffAnimation->start(false);
}

bool isNotBlack(LedColor color){
    return !(color.r == 0 && color.g == 0 && color.b == 0);
}

void LedWS1228bDeviceType::updateAveragedColor(LedStripContentIndex content)
{
    int avgRed = 0;
    int avgGreen = 0;
    int avgBlue = 0;

    int skipped = 0;

    for(uint16_t i = 0 ; i < virtualDiodesCount ; i ++)
    {
        if(isNotBlack(stripContent[content][i])){
            avgRed += stripContent[content][i].r;
            avgGreen += stripContent[content][i].g;
            avgBlue += stripContent[content][i].b;
        }else {
            skipped++;
        }
    }

    avgRed = (int)((float)avgRed/(float)(virtualDiodesCount - skipped));
    avgGreen = (int)((float)avgGreen/(float)(virtualDiodesCount - skipped));
    avgBlue = (int)((float)avgBlue/(float)(virtualDiodesCount - skipped));

    averagedColors[content].r =  (uint8_t)avgRed;
    averagedColors[content].g =  (uint8_t)avgGreen;
    averagedColors[content].b =  (uint8_t)avgBlue;

}


uint16_t LedWS1228bDeviceType::getExtendedMemoryLength(){
    return ((virtualDiodesCount * eDIFFERENT_CONTENTS_COUNT) * sizeof(LedColor));
}

uint8_t LedWS1228bDeviceType::getDeviceIdentifier(){
    return deviceId;
}
uint8_t LedWS1228bDeviceType::getDeviceType(){
    return type_LED_STRIP;
}

ServiceRequestErrorCode LedWS1228bDeviceType::service(DeviceServicesType serviceType){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode LedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        case DEVSERVICE_STATE_SWITCH:
            if(param.a == 1) {
                stripOn();
                isOn = true;
            }
            else {
                stripOff();
                isOn = false;
            }   
            return SERV_SUCCESS;

        case DEVSERVICE_LED_STRIP_SAVE_CONTENT:
            Serial.println("LedWS1228bDeviceType://DEVSERVICE_LED_STRIP_SAVE_CONTENT");
            return saveContentAs((LedStripContentIndex)param.a);
        
        case DEVSERVICE_LED_STRIP_SWITCH_CONTENT:
            Serial.println("LedWS1228bDeviceType://DEVSERVICE_LED_STRIP_SWITCH_CONTENT");
            return applyContent((LedStripContentIndex)param.a);

        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode LedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode LedWS1228bDeviceType::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        case DEVSERVICE_GET_ADVANCED_CONTROLS:
        case DEVSERVICE_GET_DETAILED_COLORS:
            Serial.println("Advanced controls requested with size: " + String((int)param.size) + ", virtualDiodesCount: " + String((int)virtualDiodesCount));
            
            if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor)  + sizeof(LedStripAnimationProperties) ))
            {
                // Copy properties
                Serial.println((uintptr_t)param.buff, HEX);
                memcpy(param.buff, &animationProperties, sizeof(LedStripAnimationProperties));
                param.buff += sizeof(LedStripAnimationProperties); // skip animation properties
                Serial.println((uintptr_t)param.buff, HEX);
                getDetailedColors((LedColor*)param.buff, ((param.size - sizeof(LedStripAnimationProperties)) /sizeof(LedColor)));


                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }
        case DEVSERVICE_SET_DETAILED_COLORS:
            if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor) + sizeof(LedStripAnimationProperties)))
            {
                // Serial.println("Color change requested");
                memcpy(&animationProperties, param.buff, sizeof(LedStripAnimationProperties));
                param.buff += sizeof(LedStripAnimationProperties); // skip animation properties
                setColors((LedColor*)param.buff, ((param.size - sizeof(LedStripAnimationProperties)) /sizeof(LedColor)));

                /* this action will affect NVM data */
                if(m_reportNvmDataChangedCbk){
                    m_reportNvmDataChangedCbk();
                }
                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }

        case DEVSERVICE_SET_EXT_MEMORY_PTR:
            return updateExtendedMemoryPtr(param.buff, param.size);
        default: 
            return SERV_NOT_SUPPORTED;
    };
}

void LedWS1228bDeviceType::applyVirtualToRealDiodes()
{
    for(uint8_t i = 0; i < virtualDiodesCount ; i ++){
        uint8_t adjustedIdx = isInversedOrder ? (virtualDiodesCount-(i+1)) : i;
        setHwLedStripColor(
            i,
            stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].r,
            stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].g,
            stripContent[eACTIVE_CURRENT_CONTENT][adjustedIdx].b
        );
    }

    adafruit_ws2812b->show();
}

 void LedWS1228bDeviceType::createEnablingAnimation()
 {
    switch(animationProperties.enableAnimation){
        case 0:
        ongoingAnimation = new RollInAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount,
            Direction::LeftToRight
        );
        break;
        case 1:
        ongoingAnimation = new FadeInAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount
        );
        break;
        case 2:
        ongoingAnimation = new SparkleInAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount
        );
        case 3:
        ongoingAnimation = new TwinkleInAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount
        );
        break;
        case 4:
        ongoingAnimation = new BounceInAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount,
            (diodesCount / 2)
        );
        break;
    }

    updateAnimationSpeed();

 }
void LedWS1228bDeviceType::createDisablingAnimation()
{
    switch(animationProperties.disableAnimation){
        case 0:
        switchOffAnimation = new FadeOutAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount
        );
        break;
        case 1:
        switchOffAnimation = new RollOutAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount,
            Direction::LeftToRight
        );
        break;
        case 2:
        switchOffAnimation = new RollOutAnimation(
            stripContent[eACTIVE_CURRENT_CONTENT],
            virtualDiodesCount,
            Direction::RightToLeft
        );
        break;
    }

    updateAnimationSpeed();
}

void LedWS1228bDeviceType::updateAnimationSpeed()
{
    switch(animationProperties.animationSpeed){
        case 0:
        animationWaitTicks = 3;
        break;
        case 1:
        animationWaitTicks = 1;
        break;
        case 2:
        animationWaitTicks = 6;
        break;
    }
    ticksToAnimate = animationWaitTicks; // reset ticks to animate
}


void LedWS1228bDeviceType::setHwLedStripColor(
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

DeviceDescription LedWS1228bDeviceType::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = isOn;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

    desc.customBytes[0] = virtualDiodesCount;

    desc.customBytes[2] = averagedColors[eACTIVE_CURRENT_CONTENT].r; // average color R
    desc.customBytes[3] = averagedColors[eACTIVE_CURRENT_CONTENT].g; // average color G
    desc.customBytes[4] = averagedColors[eACTIVE_CURRENT_CONTENT].b; // average color B

    desc.customBytes[5] = averagedColors[eSAVED_CONTENT_SLOT1].r; // average color R
    desc.customBytes[6] = averagedColors[eSAVED_CONTENT_SLOT1].g; // average color G
    desc.customBytes[7] = averagedColors[eSAVED_CONTENT_SLOT1].b; // average color B

    desc.customBytes[8] = averagedColors[eSAVED_CONTENT_SLOT2].r; // average color R
    desc.customBytes[9] = averagedColors[eSAVED_CONTENT_SLOT2].g; // average color G
    desc.customBytes[10] = averagedColors[eSAVED_CONTENT_SLOT2].b; // average color B

    desc.customBytes[11] = averagedColors[eSAVED_CONTENT_SLOT3].r; // average color R
    desc.customBytes[12] = averagedColors[eSAVED_CONTENT_SLOT3].g; // average color G
    desc.customBytes[13] = averagedColors[eSAVED_CONTENT_SLOT3].b; // average color B

    return desc;
}

#endif