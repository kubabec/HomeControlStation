#include "devices/LedWS1228b.hpp"


LedWS1228bDeviceType::LedWS1228bDeviceType(DeviceConfigSlotType nvmData)
{
    adafruit_ws2812b = new Adafruit_NeoPixel(
        nvmData.customBytes[0], /* leds count */
        nvmData.pinNumber,
        NEO_GRB + NEO_KHZ800
    );

    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;
}

void LedWS1228bDeviceType::init(){

}

void LedWS1228bDeviceType::cyclic(){

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