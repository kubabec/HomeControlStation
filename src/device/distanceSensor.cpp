#include "devices/distanceSensor.hpp"

DistanceSensor::DistanceSensor(){
   
    
}

DistanceSensor::DistanceSensor(DeviceConfigSlotType nvmData){
    pinNumber = nvmData.pinNumber;
    deviceId = nvmData.deviceId;
    deviceName = String(nvmData.deviceName);
    roomId = nvmData.roomId;

    
}


void DistanceSensor::init(){

}

void DistanceSensor::cyclic(){

}

uint16_t DistanceSensor::getExtendedMemoryLength(){
    return 0;
}

uint8_t DistanceSensor::getDeviceIdentifier(){
    return deviceId;
}
uint8_t DistanceSensor::getDeviceType(){
    return type_DISTANCE_SENSOR;
}

ServiceRequestErrorCode DistanceSensor::service(DeviceServicesType serviceType){
    switch(serviceType){
        case DEVSERVICE_ENABLE:
            /* do somethign */
            break;
        case DEVSERVICE_DISABLE:
            /* do something*/
            break;

        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode DistanceSensor::service(DeviceServicesType serviceType, ServiceParameters_set1 param){
    switch(serviceType){
        case DEVSERVICE_SET_DATA:
            /* data = param.a ;*/
            break;
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode DistanceSensor::service(DeviceServicesType serviceType, ServiceParameters_set2 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}
ServiceRequestErrorCode DistanceSensor::service(DeviceServicesType serviceType, ServiceParameters_set3 param){
    switch(serviceType){
        default: 
            return SERV_NOT_SUPPORTED;
    };
}

DeviceDescription DistanceSensor::getDeviceDescription(){
    DeviceDescription desc;
    desc.deviceType = type_DISTANCE_SENSOR;
    desc.deviceId = deviceId;
    desc.roomId = roomId;
    desc.isEnabled = true;
    desc.deviceName = deviceName;
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    desc.customBytes[0] = 100;
    return desc;
}