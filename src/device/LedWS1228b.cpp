#include "devices/LedWS1228b.hpp"


LedWS1228bDeviceType::LedWS1228bDeviceType(DeviceConfigSlotType nvmData)
{
    int count = NUMBER_OF_DIODES;

    adafruit_ws2812b = new Adafruit_NeoPixel(
        // nvmData.customBytes[0], /* leds count */
        // nvmData.pinNumber,
        count,
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


    

    for(int i = 0; i < count; i++)
    {
        // adafruit_ws2812b->setPixelColor(i, adafruit_ws2812b->Color(100, 150, 190));
        // delay(30);
        // adafruit_ws2812b->show();

        stripContent[i] = {
            .r = 130,
            .g = 70,
            .b = 170
        };

    }

    stripContent[0] = {
        .r = 200,
        .g = 100,
        .b = 50
    };
    stripContent[NUMBER_OF_DIODES-1] = {
        .r = 100,
        .g = 200,
        .b = 60
    };

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
    for(uint16_t i = 0 ; i < NUMBER_OF_DIODES ; i ++){
        adafruit_ws2812b->setPixelColor(
            i,
            adafruit_ws2812b->Color(
                stripContent[i].r,
                stripContent[i].g,
                stripContent[i].b
            )
        );
    }
    adafruit_ws2812b->show();
}

void LedWS1228bDeviceType::setColors(LedColor* ledsArray, uint16_t count)
{
    if(count == NUMBER_OF_DIODES){
        memcpy(stripContent, ledsArray, (count*sizeof(LedColor)));
        // LedColor* currentDiode = ledsArray;
        for(uint16_t i = 0; i < count; i++){
            Serial.println("R:"+ String((int)ledsArray[i].r)+" G:"+String((int)ledsArray[i].g)+" B:"+String((int)ledsArray[i].b));
        }
        applyColors();
    }
}

void LedWS1228bDeviceType::getDetailedColors(LedColor* memoryBuffer, uint16_t count)
{
    if(count == NUMBER_OF_DIODES){
        memcpy(memoryBuffer, stripContent, (count*sizeof(LedColor)));
    }  
}


uint16_t LedWS1228bDeviceType::getExtendedMemoryLength(){
    return 600;
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
            if(param.a ==1) {
                isOn = true;
            }
            else {
                isOn = false;
            }   
            return SERV_SUCCESS;

        case DEVSERVICE_BRIGHTNESS_CHANGE:
            brightness = param.a;
            return SERV_SUCCESS;

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
            if(isStripInitialized() && param.size == (NUMBER_OF_DIODES*sizeof(LedColor)))
            {
                getDetailedColors((LedColor*)param.buff, (param.size/sizeof(LedColor)));
                return SERV_SUCCESS;
            }else {
                return SERV_EXECUTION_FAILURE;
            }
        case DEVSERVICE_SET_DETAILED_COLORS:
            Serial.println("param.size: " + String((int)param.size) + ", must be : " + String((int)NUMBER_OF_DIODES*sizeof(LedColor)));
            if(isStripInitialized() && param.size == (NUMBER_OF_DIODES*sizeof(LedColor)))
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


    desc.customBytes[1] = brightness; // brightness
    desc.customBytes[2] = 200; // average color R
    desc.customBytes[3] = 100; // average color G
    desc.customBytes[4] = 0;   // average color B
    return desc;
}