#include "devices/tempSensorDHT11.hpp"


TempSensorDHT11DeviceType::TempSensorDHT11DeviceType(DeviceConfigSlotType nvmData)
{
    isOn = false;
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    currentHumid = nvmData.deviceId + 23;
    currentTemp = nvmData.deviceId - 7;
}

void TempSensorDHT11DeviceType::init(){

}

void TempSensorDHT11DeviceType::cyclic(){

    if(millis() - lastDataUpdateTime > 4000){
        if(currentHumid < 100){
            currentHumid++;
        }else {
            currentHumid = 0;
        }

        if(currentTemp < 40.f){
            currentTemp += 0.5f;
        }else {
            currentTemp = -10.f;
        }

        lastDataUpdateTime = millis();
    }

}

uint8_t TempSensorDHT11DeviceType::getDeviceIdentifier(){
    return deviceId;
}
uint8_t TempSensorDHT11DeviceType::getDeviceType(){
    return type_TEMP_SENSOR;
}

ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode TempSensorDHT11DeviceType::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription TempSensorDHT11DeviceType::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = getDeviceType();
    desc.deviceId = getDeviceIdentifier();
    desc.roomId = roomId;
    desc.isEnabled = true;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);

    desc.customBytes[2] = currentHumid; // humidity
    memcpy(&desc.customBytes[3], &currentTemp, sizeof(currentTemp));
    //desc.customBytes[3] = 73; // average color G
    return desc;
}