#include "devices/distanceSensor.hpp"


DistanceSensor::DistanceSensor(DeviceConfigSlotType nvmData){
   
    
}


void init(){

}

void cyclic(){

}

uint16_t DistanceSensor::getExtendedMemoryLength(){
    return 0;
}

uint8_t DistanceSensor::getDeviceIdentifier(){
    return 11;
}
uint8_t DistanceSensor::getDeviceType(){
    return 5;
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
    desc.deviceType = 5;
    desc.deviceId = 11;
    desc.roomId = 1;
    desc.isEnabled = true;
    desc.deviceName = "DistanceSensor";
    memset(desc.customBytes, 0x00, NUMBER_OF_CUSTOM_BYTES_IN_DESCRIPTION);
    return desc;
}