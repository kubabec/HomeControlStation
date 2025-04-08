#include "devices/LedStrip/LedWS1228b.hpp"
#ifdef LED_STRIP_SUPPORTED
const uint8_t maxVirtualLeds = 100;

LedWS1228bDeviceType::LedWS1228bDeviceType(DeviceConfigSlotType nvmData)
{
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

    adafruit_ws2812b = new Adafruit_NeoPixel(
        // nvmData.customBytes[0], /* leds count */
        // nvmData.pinNumber,
        diodesCount,
        pinNumber,
        NEO_GRB + NEO_KHZ800
    );

    adafruit_ws2812b->begin();

    // /*brightness limitation to do not exceed board current limiter */
    // const float maxCurrentAllowed = 2.6f; /* 2.6 A*/
    // const float maxCurrentPerDiode = 0.060f; /* 60mA */
    // const float maxBrightnessVal = 255.f;
    // const float numberOfDiodesF = (float)diodesCount;

    // float requiredCurrentForDiodes = maxCurrentPerDiode * numberOfDiodesF;
    // if(requiredCurrentForDiodes <= maxCurrentAllowed)
    // {
    //     /* not limited */
    //     adafruit_ws2812b->setBrightness(255);
    // }else {
    //     // float overLimit = (requiredCurrentForDiodes - maxCurrentAllowed);
    //     // float percentageOverLimit = (overLimit/maxCurrentAllowed);
    //     // float pwmValueToReduce = 255.f * percentageOverLimit;
    //     // int finalPwmValue = 255 - (int)pwmValueToReduce;
    //     adafruit_ws2812b->setBrightness(65);
    // }
    Serial.println("LedWS1228bDeviceType:// Applying current limit to : " +String((int)nvmData.customBytes[3]) + " for device id : " + String((int)deviceId));
    adafruit_ws2812b->setBrightness(nvmData.customBytes[3]); /* current limiter usage */


    // applyColors();
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

}

bool LedWS1228bDeviceType::isStripInitialized()
{
    return (adafruit_ws2812b != nullptr);
}

void LedWS1228bDeviceType::applyColors(){
    if(isContentInitialized){
        // for(uint16_t i = 0 ; i < diodesCount ; i ++){
        //     adafruit_ws2812b->setPixelColor(
        //         i,
        //         adafruit_ws2812b->Color(
        //             stripContent[eACTIVE_CURRENT_CONTENT][i].r,
        //             stripContent[eACTIVE_CURRENT_CONTENT][i].g,
        //             stripContent[eACTIVE_CURRENT_CONTENT][i].b
        //         )
        //     );
        // }

        for(uint8_t i = 0; i < virtualDiodesCount ; i ++){
            setHwLedStripColor(i);
        }

        adafruit_ws2812b->show();
        isOn = true;

        updateAveragedColor(eACTIVE_CURRENT_CONTENT);
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
    }

    return retVal;
}

void LedWS1228bDeviceType::stripOn()
{
    applyColors();
}

void LedWS1228bDeviceType::stripOff()
{
    for(uint16_t i = 0 ; i < diodesCount ; i ++){
        adafruit_ws2812b->setPixelColor(
            i,
            adafruit_ws2812b->Color( /* black => disabled */
                0x00,
                0x00,
                0x00
            )
        );
    }
    adafruit_ws2812b->show();
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
                isOn = true;
                stripOn();
            }
            else {
                isOn = false;
                stripOff();
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
            if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor)))
            {
                getDetailedColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }
        case DEVSERVICE_SET_DETAILED_COLORS:
            Serial.println("param.size: " + String((int)param.size) + ", must be : " + String((int)virtualDiodesCount*sizeof(LedColor)));
            if(isStripInitialized() && param.size == (virtualDiodesCount*sizeof(LedColor)))
            {
                Serial.println("Color change requested");
                setColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
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

void LedWS1228bDeviceType::setHwLedStripColor(uint8_t virtualLedIndex)
{
    if(virtualDiodesCount == diodesCount){
        adafruit_ws2812b->setPixelColor(
            virtualLedIndex,
            adafruit_ws2812b->Color(
                stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].r,
                stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].g,
                stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].b
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
                        stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].r,
                        stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].g,
                        stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].b
                    )
                );
            }
        }

        for(uint8_t j = 0; j < physicalLedsPerVirtualLed; j++){
            adafruit_ws2812b->setPixelColor(
                (virtualLedIndex * physicalLedsPerVirtualLed + reminderAdjustment + j),
                adafruit_ws2812b->Color(
                    stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].r,
                    stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].g,
                    stripContent[eACTIVE_CURRENT_CONTENT][virtualLedIndex].b
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