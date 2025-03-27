#include "devices/LedWS1228b.hpp"


LedWS1228bDeviceType::LedWS1228bDeviceType(DeviceConfigSlotType nvmData)
{
    diodesCount = NUMBER_OF_DIODES;

    for(uint8_t i = eACTIVE_CURRENT_CONTENT; i < eDIFFERENT_CONTENTS_COUNT; i++){
        /* Initialize each content with 0 for every color */
        memset(stripContent[i], 0x00, (diodesCount * sizeof(LedColor)));
    }

    adafruit_ws2812b = new Adafruit_NeoPixel(
        // nvmData.customBytes[0], /* leds count */
        // nvmData.pinNumber,
        diodesCount,
        40,
        NEO_GRB + NEO_KHZ800
    );

    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    adafruit_ws2812b->begin();

    /*brightness limitation to do not exceed board current limiter */
    const float maxCurrentAllowed = 2.6f; /* 2.6 A*/
    const float maxCurrentPerDiode = 0.060f; /* 60mA */
    const float maxBrightnessVal = 255.f;
    const float numberOfDiodesF = (float)NUMBER_OF_DIODES;

    float requiredCurrentForDiodes = maxCurrentPerDiode * numberOfDiodesF;
    if(requiredCurrentForDiodes <= maxCurrentAllowed)
    {
        /* not limited */
        adafruit_ws2812b->setBrightness(255);
    }else {
        // float overLimit = (requiredCurrentForDiodes - maxCurrentAllowed);
        // float percentageOverLimit = (overLimit/maxCurrentAllowed);
        // float pwmValueToReduce = 255.f * percentageOverLimit;
        // int finalPwmValue = 255 - (int)pwmValueToReduce;
        adafruit_ws2812b->setBrightness(60);
    }


    

    for(int i = 0; i < diodesCount; i++)
    {

        stripContent[eACTIVE_CURRENT_CONTENT][i] = {
            .r = 130,
            .g = 70,
            .b = 170
        };

    }

    applyColors();
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
    for(uint16_t i = 0 ; i < diodesCount ; i ++){
        adafruit_ws2812b->setPixelColor(
            i,
            adafruit_ws2812b->Color(
                stripContent[eACTIVE_CURRENT_CONTENT][i].r,
                stripContent[eACTIVE_CURRENT_CONTENT][i].g,
                stripContent[eACTIVE_CURRENT_CONTENT][i].b
            )
        );
    }
    adafruit_ws2812b->show();
    isOn = true;

    updateAveragedColor();
}

void LedWS1228bDeviceType::setColors(LedColor* ledsArray, uint16_t count)
{
    if(count == diodesCount){
        memcpy(stripContent[eACTIVE_CURRENT_CONTENT], ledsArray, (count*sizeof(LedColor)));
        applyColors();
    }
}

void LedWS1228bDeviceType::getDetailedColors(LedColor* memoryBuffer, uint16_t count)
{
    if(count == diodesCount){
        memcpy(memoryBuffer, stripContent, (count*sizeof(LedColor)));
    }  
}

ServiceRequestErrorCode LedWS1228bDeviceType::applyContent(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eACTIVE_CURRENT_CONTENT)
    {
        memcpy( /* copy data from requested content to active content */
            stripContent[eACTIVE_CURRENT_CONTENT], 
            stripContent[contentIndex],
            (diodesCount*sizeof(LedColor))
        );

        applyColors();
        retVal = SERV_SUCCESS;
    }

    return retVal;
}


ServiceRequestErrorCode LedWS1228bDeviceType::saveContentAs(LedStripContentIndex contentIndex)
{
    ServiceRequestErrorCode retVal = SERV_EXECUTION_FAILURE;

    if(contentIndex > eACTIVE_CURRENT_CONTENT && contentIndex < eACTIVE_CURRENT_CONTENT)
    {
        memcpy( /* copy data from active content to choosen content slot */ 
            stripContent[contentIndex],
            stripContent[eACTIVE_CURRENT_CONTENT],
            (diodesCount*sizeof(LedColor))
        );

        memoryAveragedColors[contentIndex-1] = averagedColor;

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

void LedWS1228bDeviceType::updateAveragedColor()
{
    int avgRed = 0;
    int avgGreen = 0;
    int avgBlue = 0;

    for(uint16_t i = 0 ; i < diodesCount ; i ++)
    {
        avgRed += stripContent[eACTIVE_CURRENT_CONTENT][i].r;
        avgGreen += stripContent[eACTIVE_CURRENT_CONTENT][i].g;
        avgBlue += stripContent[eACTIVE_CURRENT_CONTENT][i].b;
    }

    avgRed = (int)((float)avgRed/(float)diodesCount);
    avgGreen = (int)((float)avgGreen/(float)diodesCount);
    avgBlue = (int)((float)avgBlue/(float)diodesCount);

    averagedColor.r =  (uint8_t)avgRed;
    averagedColor.g =  (uint8_t)avgGreen;
    averagedColor.b =  (uint8_t)avgBlue;

}


uint16_t LedWS1228bDeviceType::getExtendedMemoryLength(){
    return ((diodesCount * eDIFFERENT_CONTENTS_COUNT)*sizeof(LedColor));
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
            return saveContentAs((LedStripContentIndex)param.a);
        
        case DEVSERVICE_LED_STRIP_SWITCH_CONTENT:
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
            if(isStripInitialized() && param.size == (diodesCount*sizeof(LedColor)))
            {
                getDetailedColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }
        case DEVSERVICE_SET_DETAILED_COLORS:
            Serial.println("param.size: " + String((int)param.size) + ", must be : " + String((int)diodesCount*sizeof(LedColor)));
            if(isStripInitialized() && param.size == (diodesCount*sizeof(LedColor)))
            {
                Serial.println("Color change requested");
                setColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }
        default: 
            return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription LedWS1228bDeviceType::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = isOn;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);


    desc.customBytes[2] = averagedColor.r; // average color R
    desc.customBytes[3] = averagedColor.g; // average color G
    desc.customBytes[4] = averagedColor.b; // average color B

    desc.customBytes[5] = memoryAveragedColors[eSAVED_CONTENT_SLOT1-1].r; // average color R
    desc.customBytes[6] = memoryAveragedColors[eSAVED_CONTENT_SLOT1-1].g; // average color G
    desc.customBytes[7] = memoryAveragedColors[eSAVED_CONTENT_SLOT1-1].b; // average color B

    desc.customBytes[8] = memoryAveragedColors[eSAVED_CONTENT_SLOT2-1].r; // average color R
    desc.customBytes[9] = memoryAveragedColors[eSAVED_CONTENT_SLOT2-1].g; // average color G
    desc.customBytes[10] = memoryAveragedColors[eSAVED_CONTENT_SLOT2-1].b; // average color B

    desc.customBytes[11] = memoryAveragedColors[eSAVED_CONTENT_SLOT3-1].r; // average color R
    desc.customBytes[12] = memoryAveragedColors[eSAVED_CONTENT_SLOT3-1].g; // average color G
    desc.customBytes[13] = memoryAveragedColors[eSAVED_CONTENT_SLOT3-1].b; // average color B

    return desc;
}